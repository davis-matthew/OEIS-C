/*
 * A162735  An alternating sum of all numbers from prime(n) to prime(n+1).
 * https://oeis.org/A162735
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{j=prime(n)..prime(n+1)} (-1)^(j+1)*j
 * Range:     u8 n <= 54, u16 n <= 6542, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation lazily builds primes up to the required index
   using trial division with the primes that have already been found.
   The prime list is cached in static storage, so any order of calls works.
   The sum is accumulated in a signed type and then cast to seq_t. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 1u)                /* the sequence is defined for n≥1 */
        return (seq_t)0;

    /* --------------------- prime cache --------------------- */
    static uint64_t *primes      = NULL;   /* array of found primes   */
    static size_t   primes_cnt   = 0;      /* number of primes stored */
    static size_t   primes_cap   = 0;      /* current allocated capacity */
    static uint64_t next_candidate = 2;    /* next integer to test */

    /* Ensure that at least n+1 primes are available */
    while (primes_cnt < (size_t)(n + 1u)) {
        /* test next_candidate for primality */
        bool is_p = true;
        for (size_t i = 0; i < primes_cnt; ++i) {
            uint64_t p = primes[i];
            if (p * p > next_candidate)
                break;
            if (next_candidate % p == 0u) {
                is_p = false;
                break;
            }
        }
        if (is_p) {
            /* grow the array if needed */
            if (primes_cap == 0u) {
                primes_cap = 16;
                primes = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
                if (!primes) return (seq_t)0;  /* out of memory */
            } else if (primes_cnt == primes_cap) {
                primes_cap *= 2;
                uint64_t *tmp = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
                if (!tmp) return (seq_t)0;      /* out of memory */
                primes = tmp;
            }
            primes[primes_cnt++] = next_candidate;
        }
        ++next_candidate;
    }

    /* --------------------- compute the alternating sum --------------------- */
    uint64_t p_n      = primes[n - 1u];  /* prime(n)   */
    uint64_t p_n_plus = primes[n    ];  /* prime(n+1) */

    long long sum = 0;                   /* signed temporary */
    for (uint64_t k = p_n; k <= p_n_plus; ++k) {
        if ((k & 1u) != 0u)              /* odd: positive */
            sum += (long long)k;
        else                             /* even: negative */
            sum -= (long long)k;
    }

    /* sum should be non‑negative for all n ≥ 1 */
    return (seq_t)sum;
}
