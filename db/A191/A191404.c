/*
 * A191404  a(n) = A000201(n) + A000201(n+3).
 * https://oeis.org/A191404
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*φ) + floor((n+3)*φ) where φ = (1+√5)/2
 * Range:     u8 n <= 0, u16 n <= 54, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Convert index to seq_t */
    seq_t n_seq       = (seq_t)n;
    seq_t n_plus3_seq = n_seq + (seq_t)3;

    /* floor(n*φ) = (n + floor(n*√5)) / 2 */
    seq_t s1 = floor_sqrt((seq_t)5 * n_seq * n_seq);
    seq_t w1 = (n_seq + s1) / (seq_t)2;

    /* floor((n+3)*φ) = (n+3 + floor((n+3)*√5)) / 2 */
    seq_t s2 = floor_sqrt((seq_t)5 * n_plus3_seq * n_plus3_seq);
    seq_t w2 = (n_plus3_seq + s2) / (seq_t)2;

    return w1 + w2;
}
