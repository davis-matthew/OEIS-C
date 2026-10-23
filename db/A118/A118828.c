/*
 * A118828  Numerators of the convergents of the 2-adic continued fraction of
 *          zero given by A118827.
 * https://oeis.org/A118828
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, sign, easy
 * Formula:   a(n) = periodic sequence with period 8: [1, -1, 0, -1, -1, 1, 0,
 *            1]
 * Range:     i8 n <= 108, i16 n <= 108, i32 n <= 108, i64 n <= 108,
 *            i128 n <= 108, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Periodic pattern (length 8) */
    static const seq_t pat[8] = {1, -1, 0, -1, -1, 1, 0, 1};
    /* n >= 1; use &7 for fast modulo 8 */
    return pat[(n - 1) & 7];
}
