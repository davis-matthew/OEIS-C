/*
 * A040350  Continued fraction for sqrt(370).
 * https://oeis.org/A040350
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0)=19; for n>0, a(n)=4 when (n-1)%3<2, otherwise a(n)=38 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)19;
    idx_t rem = (n - 1) % 3;
    return (rem == 2) ? (seq_t)38 : (seq_t)4;
}
