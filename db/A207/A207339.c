/*
 * A207339  Triangular numbers T from A000217 such that (4*T+1)/5 is prime.
 * https://oeis.org/A207339
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 34, u32 n <= 75, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A207339: Triangular numbers T such that (4*T+1)/5 is prime
   The function returns the n‑th term of that sequence (n ≥ 1). */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t n) */
#include "../utils/seq_alloc.h"      /* seq_t *seq_alloc(size_t) */
#include "../utils/seq_realloc.h"    /* seq_t *seq_realloc(seq_t*, size_t, size_t) */

seq_t a(idx_t n)
{
    /* static memoisation table for found triangular numbers */
    static seq_t *cache  = NULL;   /* array of triangular numbers */
    static size_t  used  = 0;      /* number of elements stored */
    static size_t  cap   = 0;      /* current capacity of cache */
    static idx_t   next_k = 1;     /* next index k to test */

    /* extend cache until we have at least n terms */
    while (used < n)
    {
        /* compute T_k = k*(k+1)/2 using seq_t arithmetic */
        seq_t kk  = (seq_t)next_k;
        seq_t T   = (kk * (kk + 1)) / 2;

        /* compute x = 4*T+1 */
        seq_t x   = (T * 4) + 1;

        /* check divisibility by 5; if true, evaluate the prime candidate */
        if ((x % 5) == 0)
        {
            seq_t p = x / 5;
            if (is_prime_big(p))
            {
                /* grow cache if necessary */
                if (used == cap)
                {
                    size_t newcap = (cap == 0) ? 16 : cap * 2;
                    if (cap == 0)
                        cache = seq_alloc(newcap);
                    else
                        cache = seq_realloc(cache, cap, newcap);
                    cap = newcap;
                }
                cache[used++] = T;
            }
        }
        ++next_k;
    }
    /* return the n‑th triangular number (offset 1) */
    return cache[n - 1];
}
