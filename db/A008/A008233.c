/*
 * A008233  a(n) = floor(n/4)*floor((n+1)/4)*floor((n+2)/4)*floor((n+3)/4).
 * https://oeis.org/A008233
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = floor(n/4)*floor((n+1)/4)*floor((n+2)/4)*floor((n+3)/4)
 * Range:     u8 n <= 15, u16 n <= 63, u32 n <= 1023, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t q0 = n / 4;
    idx_t q1 = (n + 1) / 4;
    idx_t q2 = (n + 2) / 4;
    idx_t q3 = (n + 3) / 4;
    return (seq_t)q0 * (seq_t)q1 * (seq_t)q2 * (seq_t)q3;
}
