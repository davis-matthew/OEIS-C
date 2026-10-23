/*
 * A155581  a(n)=If[IntegerQ[((6*n - 4)/( n + 1))*a(n - 1)], ((6*n - 4)/(n +
 *          1))* a(n - 1), If[IntegerQ[((4*n - 2)/(n + 1))*a( n - 1)], ((4*n -
 *          2)/(n + 1))*a(n - 1), n*a(n - 1)]].
 * https://oeis.org/A155581
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, uned
 * Formula:   a(n) = If[IntegerQ[((6*n - 4)/(n + 1))*a(n - 1)], ((6*n - 4)/(n
 *            + 1))*a(n - 1), If[IntegerQ[((4*n - 2)/(n + 1))*a(n - 1)], ((4*n
 *            - 2)/(n + 1))*a(n - 1), n*a(n - 1)]]; Initial value: a(0) = 1
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 13, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Sequence generator */
seq_t a(idx_t n)
{
    /* Static cache of computed values.  cache[i] holds a(i).   */
    static seq_t *cache    = NULL;   /* array of values */
    static idx_t  used    = 0;       /* number of values stored (next free index) */
    static idx_t  capacity= 0;       /* allocated array size   */

    /* ----------  First call: allocate for a(0)  ----------
       If cache is NULL we are being called for the first time.  A(0) = 1. */
    if (cache == NULL)
    {
        capacity = 1;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (cache == NULL) return 0;            /* allocation failed */
        cache[0] = (seq_t)1;                    /* a(0) = 1 */
        used = 1;                               /* next free slot is 1 */
    }

    /* ----------  Grow array if the requested index exceeds capacity  ---------- */
    if (n >= capacity)
    {
        idx_t new_cap = capacity;
        if (new_cap == 0) new_cap = 1;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;             /* allocation failed */
        cache = tmp;
        capacity = new_cap;
    }

    /* ----------  Compute missing terms from 'used' up to 'n'  ---------- */
    for (idx_t i = used; i <= n; ++i)
    {
        seq_t prev = cache[i-1];

        /*   The three possible multipliers  */
        seq_t num6 = ((seq_t)6 * (seq_t)i) - (seq_t)4;
        seq_t num4 = ((seq_t)4 * (seq_t)i) - (seq_t)2;
        seq_t den  = (seq_t)(i + (idx_t)1);

        seq_t val;
        seq_t prod6 = num6 * prev;

        if (prod6 % den == (seq_t)0)
        {
            val = prod6 / den;
        }
        else
        {
            seq_t prod4 = num4 * prev;
            if (prod4 % den == (seq_t)0)
            {
                val = prod4 / den;
            }
            else
            {
                val = prev * (seq_t)i;   /* fallback multiplier is n */
            }
        }

        cache[i] = val;
    }

    /* Keep track of how many values are now stored */
    if (used < n + 1) used = n + 1;

    return cache[n];
}
