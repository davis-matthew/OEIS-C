/*
 * A166409  Odd numbers corresponding to the positions of zeros in A166406.
 * https://oeis.org/A166409
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A166409: odd numbers 2n+1 such that the sum of i with Jacobi(i,2n+1)=-1
   equals the sum of i with Jacobi(i,2n+1)=+1. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/jacobi_symbol.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* out of offset – undefined for this sequence */

    /* Cache of previously found terms */
    static seq_t *cache = NULL;
    static idx_t  cache_len = 0;
    static idx_t  cache_cap = 0;
    static uint64_t last_checked_m = 1;   /* last odd number inspected */

    /* Generate terms until we have at least n of them */
    while (cache_len < n)
    {
        /* Next odd candidate */
        last_checked_m += 2;
        uint64_t m = last_checked_m;
        int64_t  sum = 0;

        if ((m & 3) == 1)   /* m ≡ 1 (mod 4) : pair symmetry */
        {
            uint64_t half = (m - 1) / 2;
            for (uint64_t i = 1; i <= half; ++i)
            {
                int js = jacobi_symbol(i, m);
                if (js != 0)
                    sum += (int64_t)m * js;   /* each pair contributes m·J(i,m) */
            }
        }
        else                /* m ≡ 3 (mod 4) : no simplification */
        {
            for (uint64_t i = 1; i < m; ++i)
            {
                int js = jacobi_symbol(i, m);
                if (js != 0)
                    sum += (int64_t)i * js;
            }
        }

        /* If the weighted sum is zero, the odd number m belongs to the sequence */
        if (sum == 0)
        {
            if (cache_len == cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE);
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)m;
        }
    }

    return cache[n - 1];
}
