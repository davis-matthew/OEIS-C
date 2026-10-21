/*
 * A208855  Array of even catheti of primitive Pythagorean triangles when read
 *          by SW-NE diagonals.
 * https://oeis.org/A208855
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) is the n‑th term of the infinite array b(x,m) = 4*(2*x-1)*m
 *            * when gcd(2*x-1,2*m)=1, otherwise 0. The array is traversed by
 *            * anti‑diagonals: for each s = x+m the pairs (x,m) with m=1..s-1
 *            are * visited in that order. This implementation builds the
 *            sequence * incrementally and memoises previ...
 * Range:     u8 n <= 59, u16 n <= 7428, u32 n <= 7428, u64 n <= 7428,
 *            u128 n <= 7428, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* memoised sequence values */
    static idx_t  size  = 0;      /* number of terms stored */
    static idx_t  cap   = 0;      /* capacity of cache */
    static idx_t  cur_s = 2;      /* current anti‑diagonal sum (x+m) */
    static idx_t  cur_m = 1;      /* current m within that diagonal */

    if (n == 0) return 0; /* offset is 1, undefined for n=0 */

    /* extend the cache until it contains at least n terms */
    while (size < n) {
        /* compute b(x,m) where x = cur_s - cur_m, m = cur_m */
        idx_t x = cur_s - cur_m;
        seq_t odd  = (seq_t)(2 * x - 1);   /* 2*x-1 */
        seq_t even = (seq_t)(2 * cur_m);   /* 2*m */

        seq_t value;
        if (gcd(odd, even) == 1)
            value = (seq_t)4 * odd * (seq_t)cur_m;
        else
            value = 0;

        /* ensure capacity */
        if (size + 1 >= cap) {
            size_t new_cap = cap ? cap * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort(); /* allocation failure */
            cache = tmp;
            cap   = new_cap;
        }

        cache[size++] = value;

        /* advance to next (x,m) pair */
        cur_m++;
        if (cur_m > cur_s - 1) {
            cur_s++;
            cur_m = 1;
        }
    }

    /* sequence is 0‑based in the cache, n is 1‑based */
    return cache[n - 1];
}
