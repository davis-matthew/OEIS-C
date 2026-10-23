/*
 * A016866  a(n) = (5n+1)^6.
 * https://oeis.org/A016866
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5n+1)^6
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 7, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t s = (seq_t)5 * (seq_t)n + 1; /* 5n+1 */
    seq_t s2 = s * s;                  /* (5n+1)^2 */
    seq_t s4 = s2 * s2;                /* (5n+1)^4 */
    seq_t s6 = s2 * s4;                /* (5n+1)^6 */
    return s6;
}
