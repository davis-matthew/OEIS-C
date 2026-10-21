/*
 * A059330  Primes p such that x^67 = 2 has no solution mod p.
 * https://oeis.org/A059330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 96, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   next_prime, powmod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A059330: primes p > 2 for which x^67 ≡ 2 (mod p) has no solution.
   For such p we must have p ≡ 1 (mod 67) and 2^((p−1)/67) ≠ 1 (mod p). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/powmod64.h"
#include "../utils/next_prime.h"

static uint64_t last_prime = 1;          /* last prime examined (≥2) */
static seq_t *vals = NULL;               /* sequence values */
static size_t vals_cnt = 0;              /* number of values stored */
static size_t vals_cap = 0;              /* capacity of vals */

static void compute_more(size_t need)
{
    while (vals_cnt < need)
    {
        /* get next prime after last_prime */
        uint64_t p = next_prime(last_prime + 1ULL);
        if (p <= 2ULL) {                 /* skip the prime 2 */
            last_prime = p;
            continue;
        }
        last_prime = p;                   /* update for next call */

        if (p % 67U != 1U) continue;      /* we only need p ≡ 1 (mod 67) */

        uint64_t exp = (p - 1ULL) / 67ULL;
        if (powmod64(2ULL, exp, p) != 1ULL)   /* 2 is not a 67th power mod p */
        {
            if (vals_cnt == vals_cap)        /* grow array if needed */
            {
                vals_cap = vals_cap ? vals_cap * 2U : 64U;
                seq_t *tmp = (seq_t *)realloc(vals, vals_cap * sizeof(seq_t));
                if (!tmp) {                  /* allocation failed – abort */
                    exit(EXIT_FAILURE);
                }
                vals = tmp;
            }
            vals[vals_cnt++] = (seq_t)p;     /* store the prime */
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* undefined for n < 1 */

    static bool initialized = false;
    if (!initialized)
    {
        vals     = NULL;
        vals_cap = 0;
        vals_cnt = 0;
        last_prime = 1ULL;                    /* start searching from 2 */
        initialized = true;
    }

    if (vals_cnt < (size_t)n)                /* generate more terms if needed */
        compute_more((size_t)n);

    return vals[(size_t)n - 1];
}
