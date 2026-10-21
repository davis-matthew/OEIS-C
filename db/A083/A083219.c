/*
 * A083219  a(n) = n - 2*floor(n/4).
 * https://oeis.org/A083219
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - 2*floor(n/4)
 * Range:     u8 n <= 411, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t s = (seq_t)n;                /* convert index to seq_t */
    seq_t q = s / (seq_t)4;           /* floor(n/4) */
    return s - (seq_t)2 * q;          /* n - 2*floor(n/4) */
}
