/*
 * A188430  a(n) is the maximum of the largest elements of all n-full sets, or
 *          0 if no such set exists.
 * https://oeis.org/A188430
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)/2 for n >= 20; precomputed values are used for 1 <=
 *            n <= 24
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* 1-indexed table of values that do not follow the simple rule */
/*   0 is unused to keep indices straight */
    static const seq_t small[25] = {
        0,  /* 0 unused */
        1,  /* a(1) */
        0,  /* a(2) */
        2,  /* a(3) */
        0,  /* a(4) */
        0,  /* a(5) */
        3,  /* a(6) */
        4,  /* a(7) */
        0,  /* a(8) */
        0,  /* a(9) */
        4,  /* a(10) */
        5,  /* a(11) */
        6,  /* a(12) */
        7,  /* a(13) */
        7,  /* a(14) */
        8,  /* a(15) */
        6,  /* a(16) */
        7,  /* a(17) */
        8,  /* a(18) */
        9,  /* a(19) */
        10, /* a(20) */
        11, /* a(21) */
        11, /* a(22) */
        12, /* a(23) */
        12  /* a(24) */
    };

    if (n <= 24)
        return small[(size_t)n];

    /* For n >= 25 the simple formula holds: ceil(n/2) == (n+1)/2 */
    return ((seq_t)n + 1) / 2;
}
