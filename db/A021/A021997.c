/*
 * A021997  Decimal expansion of 1/993.
 * https://oeis.org/A021997
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) – the nth decimal digit of 1/993, computed by long division
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const idx_t DEN = 993;            /* denominator 1/993   */
    idx_t rem = 1;                    /* starting remainder 1 */

    for (idx_t i = 0; i <= n; ++i)
    {
        idx_t dig = (rem * 10) / DEN; /* current digit   */
        rem = (rem * 10) % DEN;       /* update remainder */
        if (i == n) return (seq_t)dig;/* return as seq_t   */
    }
    /* Should never reach here, but return 0 for completeness */
    return (seq_t)0;
}
