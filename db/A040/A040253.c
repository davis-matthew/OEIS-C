/*
 * A040253  Continued fraction for sqrt(270).
 * https://oeis.org/A040253
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0)=16, a(n)=2,3,6,3,2,32 repeating for n>0
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)16;
    static const seq_t pattern[6] = {
        (seq_t)2, (seq_t)3, (seq_t)6,
        (seq_t)3, (seq_t)2, (seq_t)32
    };
    return pattern[(n - 1) % 6];
}
