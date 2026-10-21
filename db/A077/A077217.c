/*
 * A077217  Prime(k) such that the prime power with largest exponent that
 *          divides the product P(k) of composite numbers between prime(k) and
 *          prime(k+1) is an odd number, i.e., if p^r and 2^s divide P(k) then
 *          r >= s, p is an odd prime.
 * https://oeis.org/A077217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A077217 – primes p such that the largest odd prime power in the
 *            product of composites between p and the next prime has exponent
 *            at least the exponent of 2 in that product. (Computed by a
 *            search/sieve.)
 * Range:     u8 n <= 11, u16 n <= 732, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define SIEVE_LIMIT 2000000U            /* size of the sieve (2 million enough for n≈10 000) */

static uint32_t *primes = NULL;          /* list of primes up to SIEVE_LIMIT */
static size_t     primes_cnt = 0;        /* number of primes found */
static uint32_t *spf_tbl = NULL;         /* smallest prime factor array */
static bool       sieve_built = false;   /* flag to build the sieve once */

static void build_sieve(void)
{
    if (sieve_built) return;                  /* already built */

    spf_tbl = (uint32_t *)calloc(SIEVE_LIMIT + 1U, sizeof(uint32_t));
    if (!spf_tbl) abort();

    /* initial capacity for primes array */
    size_t primes_cap = 200000U;
    primes = (uint32_t *)malloc(primes_cap * sizeof(uint32_t));
    if (!primes) abort();

    for (uint32_t i = 2U; i <= SIEVE_LIMIT; ++i) {
        if (spf_tbl[i] == 0U) {                /* i is prime */
            spf_tbl[i] = i;

            if (primes_cnt == primes_cap) {
                primes_cap <<= 1U;
                uint32_t *tmp = (uint32_t *)realloc(primes, primes_cap * sizeof(uint32_t));
                if (!tmp) abort();
                primes = tmp;
            }
            primes[primes_cnt++] = i;

            if ((uint64_t)i * i <= SIEVE_LIMIT) {
                for (uint32_t j = i * i; j <= SIEVE_LIMIT; j += i) {
                    if (spf_tbl[j] == 0U) spf_tbl[j] = i;
                }
            }
        }
    }
    sieve_built = true;
}

/* dynamic array for the sequence values */
static seq_t *seq_vals = NULL;
static size_t seq_len  = 0;          /* number of terms stored */
static size_t seq_cap  = 0;          /* allocated capacity */

/* append a term, reallocating if necessary */
static void append_seq(seq_t v)
{
    if (seq_len == seq_cap) {
        seq_cap = seq_cap ? seq_cap << 1U : 64U;
        seq_t *tmp = (seq_t *)realloc(seq_vals, seq_cap * sizeof(seq_t));
        if (!tmp) abort();
        seq_vals = tmp;
    }
    seq_vals[seq_len++] = v;
}

/* index of the current prime in the primes array */
static size_t prime_pos = 0;

/* ---------- sequence generator ----------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0, but return 0 */

    build_sieve();                 /* ensure primes and spf are ready */

    /* generate terms until we have at least n of them */
    while (seq_len < (size_t)n) {
        if (prime_pos + 1 >= primes_cnt) break;   /* safety: no more primes */

        uint32_t p = primes[prime_pos];
        uint32_t q = primes[prime_pos + 1];

        uint32_t exp2         = 0U;                /* exponent of 2 in product P */
        uint32_t max_odd_exp  = 0U;                /* maximum odd prime exponent */

        /* small dynamic table for odd prime exponents */
        struct prime_exp { uint32_t prime; uint32_t exp; } odd[512];
        size_t odd_cnt = 0U;

        if (q > p + 1U) {                        /* composites exist */
            for (uint32_t x = p + 1U; x <= q - 1U; ++x) {
                uint32_t tmp = x;
                while (tmp > 1U) {
                    uint32_t f = spf_tbl[tmp];
                    uint32_t c = 0U;
                    while (tmp % f == 0U) {
                        tmp /= f;
                        ++c;
                    }
                    if (f == 2U) {
                        exp2 += c;
                    } else {
                        size_t k;
                        for (k = 0U; k < odd_cnt && odd[k].prime != f; ++k);
                        if (k == odd_cnt) {
                            odd[odd_cnt].prime = f;
                            odd[odd_cnt].exp   = c;
                            if (c > max_odd_exp) max_odd_exp = c;
                            ++odd_cnt;
                        } else {
                            odd[k].exp += c;
                            if (odd[k].exp > max_odd_exp) max_odd_exp = odd[k].exp;
                        }
                    }
                }
            }
        }
        /* decide whether to accept this prime */
        if (exp2 <= max_odd_exp) {
            append_seq((seq_t)p);
        }
        ++prime_pos;
    }

    return seq_vals[(size_t)n - 1U];
}
