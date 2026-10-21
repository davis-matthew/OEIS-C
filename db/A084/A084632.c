/*
 * A084632  Semiprimes such that when they are concatenated with their 10's
 *          complement, which also must be a semiprime, the result is a prime.
 * https://oeis.org/A084632
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 736, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   pow10_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A084632: semiprimes m such that 10^d - m is a semiprime and m‖(10^d - m) is prime.
   The function a(n) returns the n‑th term (n ≥ 1).  The procedure precomputes
   primes up to 2 000 000, then generates terms lazily, storing them in a
   static dynamic array.  All arithmetic is performed with 64‑bit or 128‑bit
   unsigned integers so the results are exact for the values that fit into
   seq_t (as required by every build).  If a term would overflow the limits
   of the algorithm the function simply stops; for the tested ranges this
   never occurs. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/pow10_uint64.h"

/* ---------------------------------------------------------- */
/*  prime sieve:  linear sieve up to 2 000 000                */
/* ---------------------------------------------------------- */
#define PRIME_LIMIT 2000000
static uint32_t primes[PRIME_LIMIT + 1];
static int primes_cnt = 0;

static void init_primes(void)
{
    static int done = 0;
    if (done) return;
    done = 1;
    uint32_t lp[PRIME_LIMIT + 1];
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (lp[i] == 0) {
            lp[i] = i;
            if (primes_cnt < PRIME_LIMIT) primes[primes_cnt++] = i;
        }
        for (int j = 0; j < primes_cnt && primes[j] <= lp[i] && i * primes[j] <= PRIME_LIMIT; ++j)
            lp[i * primes[j]] = primes[j];
    }
}

/* ---------------------------------------------------------- */
/*  Miscellaneous helpers                                      */
/* ---------------------------------------------------------- */
static int digits_uint64(uint64_t n)
{
    int d = 0;
    do { ++d; n /= 10; } while (n);
    return d;
}

/* is_semiprime using the global prime list */
static int is_semiprime_uint64(uint64_t n)
{
    int cnt = 0;
    uint64_t m = n;
    for (int i = 0; i < primes_cnt && (uint64_t)primes[i] * primes[i] <= m; ++i) {
        uint32_t p = primes[i];
        while (m % p == 0) {
            ++cnt;
            m /= p;
            if (cnt > 2) return 0;
        }
    }
    if (m > 1) ++cnt;
    return cnt == 2;
}

/* primality test for 128‑bit integer using 64‑bit primes */
static int is_prime_uint128(__uint128_t n)
{
    if (n < 2) return 0;
    for (int i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        __uint128_t pp = ( __uint128_t )p * p;
        if (pp > n) break;
        if ((uint64_t)(n % p) == 0) return 0;
    }
    return 1;
}

/* ---------------------------------------------------------- */
/*  Term storage and generation                               */
/* ---------------------------------------------------------- */
static seq_t *terms = NULL;
static size_t terms_len = 0;
static size_t terms_cap = 0;
static uint64_t last_candidate = 2;

static void grow_terms(void)
{
    size_t new_cap = terms_cap ? terms_cap * 2 : 64;
    terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    terms_cap = new_cap;
}

static void generate_next_term(void)
{
    for (;;) {
        uint64_t m = last_candidate++;
        /* skip if m would produce an overflow in pow10 */
        if (digits_uint64(m) > 18) break;          /* beyond 10^18, safe stop */
        if (!is_semiprime_uint64(m)) continue;

        int d = digits_uint64(m);
        uint64_t pow10_d = pow10_uint64(d);
        if (pow10_d <= m) continue; /* complement would be zero or negative */

        uint64_t c = pow10_d - m;
        if (!is_semiprime_uint64(c)) continue;

        /* concatenate m and c */
        int dc = digits_uint64(c);
        __uint128_t concat = ( __uint128_t )m * ( __uint128_t )pow10_uint64(dc) + c;

        if (!is_prime_uint128(concat)) continue;

        /* store term */
        if (terms_len == terms_cap) grow_terms();
        terms[terms_len++] = (seq_t)m;
        break;
    }
}

/* ---------------------------------------------------------- */
/*  Public function: a(n)                                     */
/* ---------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* out of range */
    init_primes();
    while (terms_len < (size_t)n)
        generate_next_term();
    return terms[n - 1];
}
