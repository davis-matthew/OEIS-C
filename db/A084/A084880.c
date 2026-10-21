/*
 * A084880  Number of (k,m,n)-multiantichains of multisets with k=3 and m=3.
 * https://oeis.org/A084880
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (27^n - 6*18^n + 6*14^n + 9*9^n - 18*6^n + 14*3^n)/6
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 16,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Requires the helper big_pow() to compute arbitrary‑precision powers. */
#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
#if defined(__SIZEOF_INT128__)
    /* For 64‑bit and smaller types we use a 128‑bit intermediate to
       avoid overflow of the terms: 27^n, 18^n, … 3^n.  The limit of 26
       guarantees that 3^n < 2^127 and therefore all products fit in
       __int128.  This covers the range of n for which the 64‑bit
       implementation can produce correct results. */
    if (sizeof(seq_t) <= 8 && n <= 26) {
        __int128 p27 = 1, p18 = 1, p14 = 1, p9 = 1, p6 = 1, p3 = 1;
        for (idx_t i = 0; i < n; ++i) {
            p27 *= 27;
            p18 *= 18;
            p14 *= 14;
            p9  *= 9;
            p6  *= 6;
            p3  *= 3;
        }
        __int128 num = p27;
        num -= 6 * p18;
        num += 6 * p14;
        num += 9 * p9;
        num -= 18 * p6;
        num += 14 * p3;
        return (seq_t)(num / 6);
    }
#endif
    /* General case: use arbitrary precision big_pow for other types */
    seq_t p27 = big_pow((seq_t)27, (seq_t)n);
    seq_t p18 = big_pow((seq_t)18, (seq_t)n);
    seq_t p14 = big_pow((seq_t)14, (seq_t)n);
    seq_t p9  = big_pow((seq_t)9,  (seq_t)n);
    seq_t p6  = big_pow((seq_t)6,  (seq_t)n);
    seq_t p3  = big_pow((seq_t)3,  (seq_t)n);

    seq_t num = p27;
    num -= 6 * p18;
    num += 6 * p14;
    num += 9 * p9;
    num -= 18 * p6;
    num += 14 * p3;
    return num / 6;
}
