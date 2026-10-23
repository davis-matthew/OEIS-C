/*
 * A010189  Continued fraction for sqrt(128).
 * https://oeis.org/A010189
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0) = 11; for n>0, a(n) = period[(n-1) mod 4] where period =
 *            {3,5,3,22}
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)11;
    static const seq_t period[4] = {(seq_t)3, (seq_t)5, (seq_t)3, (seq_t)22};
    return period[(n - 1) & 3U];
}
