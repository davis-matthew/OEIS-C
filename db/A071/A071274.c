/*
 * A071274  A071273 divided by 11.
 * https://oeis.org/A071274
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = n for 1 ≤ n ≤ 9 a(n) = 91*n - 900*floor(n/10) for n ≥ 10
 * Range:     u8 n <= 12, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 10)
        return (seq_t)n;

    seq_t sn       = (seq_t)n;
    seq_t decade   = sn / (seq_t)10;      /* floor(n/10) */
    return (seq_t)91 * sn - (seq_t)900 * decade;
}
