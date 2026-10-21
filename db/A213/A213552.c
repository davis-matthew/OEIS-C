/*
 * A213552  Principal diagonal of the convolution array A213551.
 * https://oeis.org/A213552
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n+2)*(16*n^2+7*n-3)/120
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 125, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* The calculation is done with 128‑bit arithmetic when the
       built‑in type is 64 bits or smaller, otherwise we rely on the
       arbitrary‑precision arithmetic of seq_t. */
#if defined(__SIZEOF_INT128__)
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 nn = n;
        unsigned __int128 v = nn * (nn + 1) * (nn + 2) *
                              (16 * nn * nn + 7 * nn - 3);
        v /= 120;
        return (seq_t)v;
    }
#endif
    /* Fallback for arbitrary–precision seq_t types */
    seq_t val = (seq_t)n;
    seq_t t1 = val * (val + (seq_t)1) * (val + (seq_t)2);
    seq_t t2 = (seq_t)16 * val * val + (seq_t)7 * val - (seq_t)3;
    seq_t res = t1 * t2 / (seq_t)120;
    return res;
}
