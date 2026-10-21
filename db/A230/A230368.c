/*
 * A230368  A strong divisibility sequence associated with the algebraic
 *          integer 1 + i.
 * https://oeis.org/A230368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if n mod 4 ≠ 0 a(4* m) = |(-4)^m – 1| for m ≥ 1.
 *            (Equivalently: if m even → 4^m – 1; if m odd → 4^m + 1.)
 * Range:     u8 n <= 19, u16 n <= 35, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* a(1) is the first term; behaviour for n = 0 does not matter. */
    if ((n & 3) != 0)          /* not divisible by 4 */
        return (seq_t)1;

    idx_t m = n >> 2;          /* m = n / 4 */
    seq_t pow4_m = seq_pow((seq_t)4, m);   /* 4^m */

    /* For even m, (-4)^m = 4^m → |4^m – 1| = 4^m – 1.  For odd m,
       (-4)^m = –4^m → |–4^m – 1| = 4^m + 1. */
    if ((m & 1ULL) == 0)
        return pow4_m - (seq_t)1;
    else
        return pow4_m + (seq_t)1;
}
