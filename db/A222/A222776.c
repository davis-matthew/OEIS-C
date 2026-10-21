/*
 * A222776  Number of n X 1 0..6 arrays with no element equal to another at a
 *          city block distance of exactly two, and new values 0..6 introduced
 *          in row major order.
 * https://oeis.org/A222776
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 16*a(n-1) - 95*a(n-2) + 260*a(n-3) - 324*a(n-4) +
 *            144*a(n-5) for n>7 with a(1)=1, a(2)=2, a(3)=3, a(4)=7, a(5)=20,
 *            a(6)=67, a(7)=255
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 16, u64 n <= 29, u128 n <= 54,
 *            big any n
 * Helpers:   grow_seq_array, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/grow_seq_array.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* static cache: a(1) stored at cache[0] */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* capacity of cache */
    static size_t size  = 0;   /* number of terms cached (>=0) */

    if (n == 0) return 0;  /* undefined for n<1; return 0 as safe guard */

    /* ensure cache has at least 7 terms with correct initial values */
    if (size == 0) {
        grow_seq_array(&cache, &cap, 7);
        cache[0] = (seq_t)1;   /* a(1) */
        cache[1] = (seq_t)2;   /* a(2) */
        cache[2] = (seq_t)3;   /* a(3) */
        cache[3] = (seq_t)7;   /* a(4) */
        cache[4] = (seq_t)20;  /* a(5) */
        cache[5] = (seq_t)67;  /* a(6) */
        cache[6] = (seq_t)255; /* a(7) */
        size = 7;
    }

    /* compute up to requested n */
    if ((size_t)n > size) {
        /* ensure capacity */
        grow_seq_array(&cache, &cap, (size_t)n);

        /* extend cache */
        for (size_t i = size; i < (size_t)n; ++i) {   /* i is index of a(i+1) */
            seq_t a1 = cache[i-1]; /* a(n-1) */
            seq_t a2 = cache[i-2]; /* a(n-2) */
            seq_t a3 = cache[i-3]; /* a(n-3) */
            seq_t a4 = cache[i-4]; /* a(n-4) */
            seq_t a5 = cache[i-5]; /* a(n-5) */

            /* compute term using only seq_t arithmetic */
            seq_t term = mul_small(a1, 16);
            term += mul_small(a3, 260);
            term += mul_small(a5, 144);
            term -= mul_small(a2, 95);
            term -= mul_small(a4, 324);

            cache[i] = term;
        }
        size = (size_t)n;
    }

    return cache[(size_t)n - 1];
}
