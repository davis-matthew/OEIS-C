/*
 * A134977  Period 6: repeat [1, 4, 2, 3, 0, 2].
 * https://oeis.org/A134977
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = [1, 4, 2, 3, 0, 2][n mod 6]
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Period‑6 pattern */
    static const seq_t pat[6] = { (seq_t)1, (seq_t)4, (seq_t)2, (seq_t)3, (seq_t)0, (seq_t)2 };
    return pat[n % 6];
}
