/*
 * A050695  Composite numbers k such that none of the prime factors of k is a
 *          substring of k.
 * https://oeis.org/A050695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, nice
 * Formula:   Composite numbers k such that no prime factor of k appears as a
 *            decimal substring of k. The function a(n) returns the n‑th such
 *            number (n ≥ 1)
 * Range:     u8 n <= 96, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   contains_substring, u64_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/contains_substring.h"
#include "../utils/u64_to_str.h"

/* ------------------------------------------------------------ */
/* Prime list and sieve (up to a fixed limit)                  */
/* ------------------------------------------------------------ */
static uint64_t *prime_list = NULL;   /* array of primes */
static size_t prime_len  = 0;         /* number of primes stored */
static bool primes_initialized = false;

static void init_primes(void)
{
    if (primes_initialized) return;

    const uint64_t LIMIT = 2000000ULL;   /* should cover a(10 000) comfortably */

    /* boolean array for sieve */
    uint8_t *is_comp = (uint8_t *)malloc(LIMIT + 1);
    memset(is_comp, 0, LIMIT + 1);

    /* preallocate for about LIMIT/log(LIMIT) primes */
    size_t prime_cap = 200000;
    prime_list = (uint64_t *)malloc(sizeof(uint64_t) * prime_cap);
    prime_len = 0;

    for (uint64_t i = 2; i <= LIMIT; ++i) {
        if (!is_comp[i]) {
            if (prime_len >= prime_cap) {
                prime_cap <<= 1;
                prime_list = (uint64_t *)realloc(prime_list,
                            sizeof(uint64_t) * prime_cap);
            }
            prime_list[prime_len++] = i;
            if (i * i <= LIMIT) {
                for (uint64_t j = i * i; j <= LIMIT; j += i)
                    is_comp[j] = 1;
            }
        }
    }
    free(is_comp);
    primes_initialized = true;
}

/* ------------------------------------------------------------ */
/* Sequence storage                                         */
/* ------------------------------------------------------------ */
static seq_t *seq_arr = NULL;   /* array of sequence values */
static size_t seq_len = 0;      /* number of values stored */
static size_t seq_cap = 0;      /* allocated capacity */
static uint64_t last_checked = 3;   /* next candidate will be 4 */

/* ------------------------------------------------------------ */
/* Main sequence function                                    */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;       /* undefined, but safe value */

    init_primes();

    /* grow sequence array until it holds n terms */
    while (seq_len < (size_t)n) {
        ++last_checked;

        /* factorization of last_checked */
        uint64_t temp = last_checked;
        uint64_t factors[64];
        int fac_cnt = 0;

        for (size_t i = 0; i < prime_len && prime_list[i] * prime_list[i] <= temp; ++i) {
            uint64_t p = prime_list[i];
            if (temp % p == 0) {
                factors[fac_cnt++] = p;
                while (temp % p == 0)
                    temp /= p;
            }
        }
        if (temp > 1)
            factors[fac_cnt++] = temp;

        /* skip primes */
        if (fac_cnt == 1 && factors[0] == last_checked)
            continue;   /* prime number */

        /* build string of last_checked */
        char num_buf[64];
        u64_to_str(last_checked, num_buf);

        /* check that none of its prime factors is a substring of num_buf */
        bool ok = true;
        for (int k = 0; k < fac_cnt; ++k) {
            char factor_buf[64];
            u64_to_str(factors[k], factor_buf);
            if (contains_substring(num_buf, factor_buf)) {
                ok = false;
                break;
            }
        }

        if (ok) {
            /* append to seq_arr */
            if (seq_len >= seq_cap) {
                seq_cap = seq_cap ? seq_cap * 2 : 4;
                seq_arr = (seq_t *)realloc(seq_arr,
                            sizeof(seq_t) * seq_cap);
            }
            seq_arr[seq_len++] = (seq_t)last_checked;
        }
    }

    return seq_arr[seq_len - 1];   /* seq_len == n */
}
