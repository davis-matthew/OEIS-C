/*
 * A165805  Integers that start a trajectory x -> A008619(x) which contains
 *          only primes until terminating at 2 or 3.
 * https://oeis.org/A165805
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A165805 – Compute the nth integer whose floor‑halving trajectory
   contains only primes until it reaches 2 or 3.  The algorithm
   incrementally searches candidate numbers and stores the found
   sequence in a static cache.  It uses the provided helper
   is_prime_big() to test primality of arbitrary‑precision values. */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;   /* cached sequence values, 0‑based */
static idx_t  cache_len = 0;  /* number of terms cached */
static size_t cache_cap = 0;  /* allocated capacity of cache */

/* Return the n-th term of A165805 (1‑based indexing). */
seq_t a(idx_t n)
{
    if (n < 1)                /* out of range – undefined behaviour in OEIS */
        return (seq_t)0;

    /* Return from cache if already computed. */
    if (cache_len >= n)
        return cache[n - 1];

    /* Start searching from the next integer after the last cached value. */
    seq_t cand = (cache_len == 0) ? (seq_t)2 : cache[cache_len - 1] + (seq_t)1;

    while (cache_len < n)
    {
        /* Check whether cand is a valid term. */
        bool good;
        if (cand == (seq_t)2 || cand == (seq_t)3)
        {
            good = true;
        }
        else
        {
            seq_t x = cand >> 1;        /* floor(cand/2)  – integer division */
            good = true;
            while (x > (seq_t)3)
            {
                if (!is_prime_big(x))
                {
                    good = false;
                    break;
                }
                x >>= 1;               /* next in trajectory */
            }
        }

        if (good)
        {
            /* Grow cache if necessary. */
            if (cache_len == cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)
                {
                    abort(); /* memory exhaustion */
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cand;
        }

        /* Next candidate (wrap around is fine for unsigned seq_t). */
        cand = cand + (seq_t)1;
    }

    return cache[n - 1];
}
