/*
 * A056958  Number of days in months of Coptic calendar.
 * https://oeis.org/A056958
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 30 if n mod 13 ≠ 0, otherwise a(n) = 6 if year mod 4 = 0
 *            else 5, where year = ⌈n/13⌉ = (n + 12) / 13
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for offset 1, safe fallback */
    seq_t rem = (seq_t)n % 13;
    if (rem != 0) return 30;
    seq_t year = ((seq_t)n + 12) / 13;
    return (year % 4) == 0 ? 6 : 5;
}
