/*
 * A017486  a(n) = (11*n + 8)^2.
 * https://oeis.org/A017486
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*n + 8)^2
 * Range:     u8 n <= 0, u16 n <= 22, u32 n <= 5957, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t = (seq_t)n;          /* cast index to seq_t */
    seq_t v = (seq_t)11 * t + 8; /* 11*n + 8 */
    return v * v;                /* square */
}
