/*
 * A158648  Numbers n such that 30*n + 17 is prime.
 * https://oeis.org/A158648
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th non‑negative integer k such that 30*k+17 is
 *            prime (OEIS A158648). a(1)=0. The function builds a cache of
 *            found terms in increasing order; any order of calls is supported
 * Range:     u8 n <= 7, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    /* static cache of previously computed terms */
    static seq_t *cache   = NULL;   /* array of found k's */
    static size_t  cap    = 0;      /* allocated capacity */
    static size_t  sz     = 0;      /* number of terms stored */
    static idx_t   next_k = 0;      /* next candidate k to test */

    /* For n==0 the OEIS offset is 1, but we return 0 as a safe default */
    if (n == 0) return 0;

    /* Ensure the cache contains at least n terms */
    while (sz < n)
    {
        /* 30*next_k+17 safely fits into uint64_t for the intended range */
        uint64_t num = 30ULL * (uint64_t)next_k + 17ULL;
        if (is_prime_small((seq_t)num))
        {
            /* Grow the cache if necessary */
            if (sz == cap)
            {
                size_t new_cap = cap ? cap * 2 : 64;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();  /* unrecoverable allocation failure */
                cache = tmp;
                cap   = new_cap;
            }
            cache[sz++] = (seq_t)next_k;
        }
        next_k++;
    }

    return cache[n - 1];
}
