/*
 * A032775  Numbers that are congruent to {0, 1, 2, 3, 5, 6} mod 7.
 * https://oeis.org/A032775
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*floor((n-1)/6) + f((n-1)%6) with f = {0,1,2,3,5,6}
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* not defined for offset < 1, return 0 */
    idx_t m = n - 1;               /* adjust to 0‑based index */
    idx_t block = m / 6;           /* number of complete 7‑blocks skipped */
    idx_t rem   = m % 6;           /* position in the current 7‑block */
    seq_t offset = (rem < 4) ? (seq_t)rem : (seq_t)(rem + 1);
    return (seq_t)block * 7 + offset;
}
