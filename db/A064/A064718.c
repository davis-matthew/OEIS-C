/*
 * A064718  A Beatty sequence for 2^i + 2^-i where i = sqrt(-1).
 * https://oeis.org/A064718
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ⌊ (20*n - 1) / 7 ⌋ (Beatty sequence for 2^i + 2^-i)
 * Range:     u8 n <= 12, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert idx_t to seq_t for arithmetic */
    seq_t t = (seq_t)n;
    t *= (seq_t)20;   /* 20*n */
    t -= (seq_t)1;    /* 20*n - 1 */
    return t / (seq_t)7;   /* floor division */
}
