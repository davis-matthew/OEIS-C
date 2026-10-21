/*
 * A211811  Number of ordered triples (w,x,y) with all terms in {1,...,n} and
 *          2w^3>x^3+y^3.
 * https://oeis.org/A211811
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # of ordered triples (w,x,y) with 1 ≤ w,x,y ≤ n and 2·w³
 *            > x³ + y³. The code counts pairs (x,y) for each w by a
 *            two‑pointer scan, using a cached table of cubes
 * Range:     u8 n <= 5, u16 n <= 31, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache of i³ for i = 0..cache_max (cubes[0] unused) */
    static idx_t   cache_max = 0;
    static seq_t *cubes    = NULL;

    /* grow cube table if necessary */
    if (n > cache_max) {
        size_t new_size = (size_t)n + 1;
        seq_t *new_cubes = (seq_t *)realloc(cubes, new_size * sizeof(seq_t));
        if (!new_cubes) return (seq_t)0;          /* out‑of‑memory guard */
        cubes = new_cubes;
        for (idx_t i = cache_max + 1; i <= n; ++i)
            cubes[i] = (seq_t)i * (seq_t)i * (seq_t)i;   /* i³ */
        cache_max = n;
    }

    seq_t total = 0;   /* final answer a(n) */

    for (idx_t w = 1; w <= n; ++w) {
        seq_t limit = (seq_t)2 * cubes[w];   /* 2·w³, threshold for < */
        idx_t   j = n;                       /* right pointer for y     */
        seq_t   pairs = 0;                   /* count for this w        */

        for (idx_t x = 1; x <= n; ++x) {     /* x runs from 1 to n      */
            while (j >= 1 && cubes[x] + cubes[j] >= limit)
                --j;
            if (j == 0) break;              /* no y can satisfy now   */
            pairs += j;                     /* y can be 1..j          */
        }
        total += pairs;
    }

    return total;
}
