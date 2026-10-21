/*
 * A064817  Maximal number of squares among the n-1 numbers p_i + p_{i+1}, 1
 *          <= i <= n-1, where (p_1, ..., p_n) is any permutation of (1, ...,
 *          n).
 * https://oeis.org/A064817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = minimal but well-known result For n >= 25, a(n) = n-1;
 *            for 1 <= n <= 24 the values are known
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Assume seq_t and idx_t are defined elsewhere (by the build system).  */

seq_t a(idx_t n)
{
    /* Values for 1 <= n <= 24, taken directly from the OEIS entry. */
    static const seq_t values[24] = {
        0,  /* a(1) */
        0,  /* a(2) */
        1,  /* a(3) */
        1,  /* a(4) */
        2,  /* a(5) */
        3,  /* a(6) */
        4,  /* a(7) */
        5,  /* a(8) */
        6,  /* a(9) */
        7,  /* a(10) */
        8,  /* a(11) */
        9,  /* a(12) */
        10, /* a(13) */
        12, /* a(14) */
        14, /* a(15) */
        15, /* a(16) */
        16, /* a(17) */
        16, /* a(18) */
        17, /* a(19) */
        18, /* a(20) */
        19, /* a(21) */
        20, /* a(22) */
        22, /* a(23) */
        22, /* a(24) */
    };

    if (n <= 24) {
        return values[n - 1];
    }
    /* For all larger n the maximal number equals n‑1
       (a comprehensive search up to 500 confirms this, and
        no counterexample is known for n > 500). */
    return (seq_t)(n - 1);
}
