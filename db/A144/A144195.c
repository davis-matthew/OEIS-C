/*
 * A144195  Square array (6 X 6) read by rows.
 * https://oeis.org/A144195
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, tabf
 * Formula:   a(n) = 1 if (n-1)%6 equals perm[(n-1)/6] else 0, where perm =
 *            {5,0,2,3,4,1}. For n > 36 a(n)=0
 * Range:     u8 n <= 36, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Adjust for 1-based indexing. The array has 6×6 = 36 elements. */
    if (n == 0U) return 0;          /* safety: although n>=1 by specification */
    idx_t idx = n - 1U;              /* zero‑based linear index */
    idx_t row = idx / 6ULL;          /* 0‑based row (0..5) */
    if (row >= 6ULL) return 0;       /* outside the 6×6 array */

    idx_t col = idx % 6ULL;          /* 0‑based column */
    static const idx_t perm[6] = {5ULL, 0ULL, 2ULL, 3ULL, 4ULL, 1ULL};

    /* Return 1 if the column matches the permutation entry, else 0. */
    return (col == perm[row]) ? 1 : 0;
}
