/*
 * A221667  Sum of neighbor maps: number of 2 X n binary arrays indicating the
 *          locations of corresponding elements equal to the sum mod 3 of
 *          their horizontal, diagonal and antidiagonal neighbors in a random
 *          0..2 2 X n array.
 * https://oeis.org/A221667
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + 8*a(n-3) - 32*a(n-4) - 16*a(n-6) + 64*a(n-7)
 *            Initial values: a(1)=4, a(2)=12, a(3)=64, a(4)=256, a(5)=992,
 *            a(6)=4096, a(7)=16384
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 15, u64 n <= 32, u128 n <= 63,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* static cache shared among calls */
    static seq_t *cache = NULL;
    static idx_t  len   = 0;   /* number of terms stored (>=0) */
    static size_t cap   = 0;   /* allocated capacity */

    /* initialisation on first call */
    if (cache == NULL) {
        cap = 7;
        len = 7;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;         /* memory error → zero */

        cache[0] = (seq_t)4;      /* a(1) */
        cache[1] = (seq_t)12;     /* a(2) */
        cache[2] = (seq_t)64;     /* a(3) */
        cache[3] = (seq_t)256;    /* a(4) */
        cache[4] = (seq_t)992;    /* a(5) */
        cache[5] = (seq_t)4096;   /* a(6) */
        cache[6] = (seq_t)16384;  /* a(7) */
    }

    /* returned directly if already computed */
    if (n <= len) {
        return cache[(size_t)(n - 1)];
    }

    /* grow array if necessary */
    if ((size_t)n > cap) {
        size_t new_cap = cap;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;           /* memory error → zero */
        cache = tmp;
        cap   = new_cap;
    }

    /* compute missing terms using the 7‑term recurrence */
    for (idx_t cur = len + 1; cur <= n; ++cur) {
        size_t idx = (size_t)(cur - 1);        /* a(cur) -> cache[idx] */
        size_t i1  = idx - 1;   /* a(cur-1) */
        size_t i3  = idx - 3;   /* a(cur-3) */
        size_t i4  = idx - 4;   /* a(cur-4) */
        size_t i6  = idx - 6;   /* a(cur-6) */
        size_t i7  = idx - 7;   /* a(cur-7) */

        /* use mul_small to avoid intermediate overflow */
        seq_t t1 = mul_small(cache[i1], 4);
        seq_t t2 = mul_small(cache[i3], 8);
        seq_t t3 = mul_small(cache[i4], 32);
        seq_t t4 = mul_small(cache[i6], 16);
        seq_t t5 = mul_small(cache[i7], 64);

        seq_t val = t1;
        val += t2;
        val -= t3;
        val -= t4;
        val += t5;

        cache[idx] = val;
    }

    len = n;
    return cache[(size_t)(n - 1)];
}
