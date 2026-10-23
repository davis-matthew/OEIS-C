/*
 * A018801  Index of smallest triangular number beginning with n.
 * https://oeis.org/A018801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = index k of the smallest triangular number T_k * whose
 *            decimal representation starts with the decimal digits of n
 * Range:     u8 n <= 7, u16 n <= 67, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   seq_isqrt_floor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_isqrt_floor.h"

seq_t a(idx_t n)
{
    seq_t N     = (seq_t)n;
    seq_t pow10 = (seq_t)1;

    for (;;) {
        seq_t lower = N * pow10;
        seq_t upper = (N + (seq_t)1) * pow10 - (seq_t)1;

        seq_t target = lower * (seq_t)8 + (seq_t)1;
        seq_t s      = seq_isqrt_floor(target);
        seq_t k      = (s - (seq_t)1) / (seq_t)2;

        /* adjust k upward until T_k >= lower */
        while ((k * (k + (seq_t)1)) / (seq_t)2 < lower)
            ++k;

        seq_t Tk = (k * (k + (seq_t)1)) / (seq_t)2;
        if (Tk <= upper)   /* triangular number found */
            return k;

        pow10 *= (seq_t)10;   /* increase leading-digit window */
    }
}
