/*
 * A093005  a(n) = n * ceiling(n/2).
 * https://oeis.org/A093005
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * floor((n + 1) / 2) (equivalent to n * ceil(n/2))
 * Range:     u8 n <= 22, u16 n <= 362, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t s = (seq_t)n;            /* convert index to seq_t */
    seq_t t = (s + 1) / 2;         /* floor((n+1)/2) */
    return s * t;                  /* n * ceil(n/2) */
}
