/*
 * A017043  a(n) = (7*n + 5)^3.
 * https://oeis.org/A017043
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*n + 5)^3
 * Range:     u8 n <= 0, u16 n <= 5, u32 n <= 231, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t = (seq_t)n;          /* cast index to seq_t */
    seq_t base = t * (seq_t)7 + (seq_t)5;  /* 7*n + 5 */
    seq_t result = base * base * base;     /* (7*n+5)^3 */
    return result;
}
