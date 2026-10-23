/*
 * A040345  Continued fraction for sqrt(365).
 * https://oeis.org/A040345
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0)=19; for n>=1, a(n)= {9,1,1,9,38}[(n-1)%5]
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)19;
    static const seq_t block[5] = {(seq_t)9, (seq_t)1, (seq_t)1, (seq_t)9, (seq_t)38};
    return block[(n - 1) % 5];
}
