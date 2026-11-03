/*
 * A045397  Primes congruent to {0, 3, 4, 5} mod 7.
 * https://oeis.org/A045397
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 62 terms through n = 943 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes primes congruent to {0,3,4,5} (mod 7) */
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/mem.h"

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* cache of previously computed primes */
    static seq_t *cache  = NULL;
    static size_t sz     = 0;    /* number of elements stored      */
    static size_t cap    = 0;    /* allocated capacity             */

    /* next candidate to test (always ≥ 2) */
    static idx_t cand = 2;

    if (n > sz)
    {
        /* Grow the cache to hold at least n elements */
        grow_seq(&cache, &cap, n);

        /* Generate missing entries */
        for (; sz < n; ++sz)
        {
            for (;;)
            {
                /* test primality of cand */
                if (is_prime(cand))
                {
                    /* accept only if cand mod 7 ∈ {0,3,4,5} */
                    idx_t m = cand % 7;
                    if (m == 0 || m == 3 || m == 4 || m == 5)
                    {
                        cache[sz] = (seq_t)cand;      /* store value */
                        break;                        /* found next prime */
                    }
                }
                ++cand;                               /* try next candidate */
            }
            /* next search starts just after the accepted prime */
            ++cand;
        }
    }

    return cache[n - 1];
}
