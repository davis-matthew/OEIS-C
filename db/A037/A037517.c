/*
 * A037517  Base 8 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,0,1.
 * https://oeis.org/A037517
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 8*a(n-1) + a(n-3) - 8*a(n-4) with initial values a(1)=2,
 *            a(2)=16, a(3)=129, a(4)=1034
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;        /* cached sequence values */
static idx_t  computed = 0;        /* number of terms computed so far (>=1 if any) */
static size_t cap = 0;             /* current capacity of the cache */

/* Return the n-th term of A037517; n >= 1 */
seq_t a(idx_t n)
{
    /* Ensure we have capacity for n terms */
    if (n > cap)
    {
        size_t new_cap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();        /* allocation failed */
        cache = tmp;
        cap = new_cap;
    }

    /* Compute terms up to n if necessary */
    if (n > computed)
    {
        for (idx_t i = computed + 1; i <= n; ++i)
        {
            idx_t idx = i - 1;                /* 0‑based array index */
            if (i == 1)
                cache[idx] = (seq_t)2;
            else if (i == 2)
                cache[idx] = (seq_t)16;
            else if (i == 3)
                cache[idx] = (seq_t)129;
            else if (i == 4)
                cache[idx] = (seq_t)1034;
            else
                cache[idx] = ((seq_t)8 * cache[idx - 1]) +
                              cache[idx - 3] -
                              ((seq_t)8 * cache[idx - 4]);
        }
        computed = n;
    }

    return cache[n - 1];
}
