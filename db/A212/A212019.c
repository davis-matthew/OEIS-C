/*
 * A212019  Number of (w,x,y,z) with all terms in {1,...,n} and w*x=2*y*z.
 * https://oeis.org/A212019
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 120, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count quadruples (w,x,y,z) in [1..n] with w*x = 2*y*z by
   counting factorizations of all products up to n*n. */
#include <stddef.h>   /* NULL, size_t */
#include <stdint.h>   /* uint16_t */
#include <stdlib.h>   /* malloc, free, memset */
#include <string.h>   /* memset */

seq_t a(idx_t n)
{
    /* Trivial cases */
    if (n < 2)
        return 0;

    /* Product range */
    idx_t prodLimit = n * n;

    /* Static cache for pair counts */
    static uint16_t *cnt = NULL;
    static idx_t cntSize = 0;

    /* Grow cache if needed */
    if (prodLimit >= cntSize) {
        void *tmp = realloc(cnt, (size_t)(prodLimit + 1) * sizeof(uint16_t));
        if (!tmp)
            return 0;                      /* allocation failed */
        cnt = (uint16_t *)tmp;
        cntSize = prodLimit + 1;
    }

    /* Zero the relevant part of the array */
    memset(cnt, 0, (size_t)(prodLimit + 1) * sizeof(uint16_t));

    /* Count ordered pairs (y,z) for each product */
    for (idx_t y = 1; y <= n; ++y) {
        for (idx_t z = 1; z <= n; ++z) {
            /* product fits in idx_t */
            idx_t p = y * z;
            ++cnt[p];
        }
    }

    /* Sum over even products */
    seq_t res = 0;
    for (idx_t p = 2; p <= prodLimit; p += 2) {
        uint16_t f_p   = cnt[p];
        uint16_t f_half = cnt[p >> 1];          /* p/2 */
        if (f_p && f_half)
            res += (seq_t)f_p * f_half;
    }

    return res;
}
