/*
 * A008218  a(n) = floor(n/4)*floor((n+1)/4)*floor((n+2)/4).
 * https://oeis.org/A008218
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n/4)*floor((n+1)/4)*floor((n+2)/4)
 * Range:     u8 n <= 26, u16 n <= 161, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t q0 = ((seq_t)n) / 4;
    seq_t q1 = (((seq_t)n) + 1) / 4;
    seq_t q2 = (((seq_t)n) + 2) / 4;
    return q0 * q1 * q2;
}
