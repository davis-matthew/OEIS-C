/*
 * A047335  Numbers that are congruent to {0, 6} mod 7.
 * https://oeis.org/A047335
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (14*n + 5*(-1)^n - 9)/4
 * Range:     u8 n <= 19, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)14 * (seq_t)n;
    if ((n & 1) == 0)   /* n even: (-1)^n = 1 */
        base += (seq_t)5;
    else                /* n odd:  (-1)^n = -1 */
        base -= (seq_t)5;
    base -= (seq_t)9;
    return base / (seq_t)4;
}
