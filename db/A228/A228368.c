/*
 * A228368  Difference between the n-th element of the ruler function and the
 *          highest power of 2 dividing n.
 * https://oeis.org/A228368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabf
 * Formula:   a(n) = (v2(n)+1) - 2^(v2(n)) where v2(n) is the exponent of the
 *            largest power of 2 dividing n. This reproduces the OEIS sequence
 *            A228368
 * Range:     i8 n <= 255, i16 n <= 8873, i32 n <= 8873, i64 n <= 8873,
 *            i128 n <= 8873, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute v2(n): the number of trailing zeros in n's binary
       representation. For n >= 1 this loop terminates.            */
    unsigned int v = 0;
    idx_t m = n;
    while ((m & (idx_t)1) == 0) {
        v++;
        m >>= 1;
    }

    /* compute 2^v as a 128‑bit integer to avoid overflow for large v */
    __int128 pow2 = (__int128)1 << v;

    /* result = (v + 1) - 2^v */
    __int128 result = (__int128)(v + 1) - pow2;

    return (seq_t)result;
}
