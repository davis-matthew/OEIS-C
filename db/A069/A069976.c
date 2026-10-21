/*
 * A069976  Interior angle of a regular polygon of n sides, rounded to nearest
 *          integer.
 * https://oeis.org/A069976
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = round(((n-2)*180)/n)
 * Range:     u8 n <= 3, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3) return (seq_t)0;          /* undefined for n<3, return 0 */
    seq_t N   = (seq_t)n;                /* convert index to seq_t */
    seq_t num = (N - 2) * 180 + N / 2;   /* add half‑n for rounding */
    return num / N;                      /* integer division gives rounded value */
}
