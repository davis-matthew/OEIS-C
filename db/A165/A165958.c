/*
 * A165958  The digits on a number pad from lower right to upper left.
 * https://oeis.org/A165958
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, base, less
 * Formula:   a(1)=0; for n≥2, a(n)=3*ceil((n-1)/3)-((n-2) mod 3), defined for
 *            1≤n≤10
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 0;          /* first term */
    if (n < 1 || n > 10) return 0; /* outside the defined range */

    idx_t m = n - 2;               /* zero‑based index inside the 3‑element blocks */
    idx_t g = m / 3;               /* block number (0 for 3,2,1; 1 for 6,5,4; 2 for 9,8,7) */
    idx_t offset = m % 3;          /* position inside the block */
    seq_t val = (seq_t)(3 * ((seq_t)g + 1) - (seq_t)offset);
    return val;
}
