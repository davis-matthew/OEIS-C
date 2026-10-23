/*
 * A191705  Dispersion of A016873, (5k+3), by antidiagonals.
 * https://oeis.org/A191705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 28, u32 n <= 120, u64 n <= 406,
 *            u128 n <= 1596, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A191705 – Dispersion of A016873 (5k+3) by antidiagonals.
 *
 *  Each integer a(n) is obtained by
 *
 *   1. Computing the antidiagonal index (r,c) of n, where r is the row and
 *      c is the column.  Row r increases from 1 to the diagonal length D.
 *   2. The value in column 1 is
 *
 *          a_{r,1} = r + ⌊(r+1)/4⌋.
 *
 *      This gives all integers that are NOT congruent to 3 modulo 5.
 *
 *   3. For columns c ≥ 2 the values satisfy
 *
 *          a_{r,c} = a_{r,1} + (4·a_{r,1} – 2) · (5^{c-1} – 1)/4 .
 *
 *      The factor (4·a_{r,1} – 2) is the increment from column 1 to column 2.
 *      Subsequent columns add increments that are 5‑times the previous ones.
 *
 *  The function below implements these formulas using only integer
 *  arithmetic; arbitrary‑precision arithmetic is used when the build
 *  supplies such a type for seq_t.
 */

seq_t a(idx_t n)
{
    /*--- 1. Find the antidiagonal (row + col) that contains index n ----------*/
    idx_t D = 1;
    while ((unsigned __int128)D * (D + 1) / 2 < n) ++D;

    /* Number of terms before this diagonal */
    unsigned __int128 prevTri = (unsigned __int128)(D - 1) * D / 2;
    idx_t prevCnt = (idx_t)prevTri;

    /* Position k within the diagonal (1 ≤ k ≤ D) */
    idx_t k = n - prevCnt;

    /*--- 2. Compute coordinates (r,c) ---------------------------------------*/
    idx_t r = k;                 /* row number  */
    idx_t c = D - k + 1;         /* column number */

    /*--- 3. Column 1 value ---------------------------------------------------*/
    seq_t base = (seq_t)r + (seq_t)((r + 1) / 4);   /* a_{r,1} */

    /* If we are on the first column, return immediately */
    if (c == 1)
        return base;

    /*--- 4. Compute the common difference d2 = a_{r,2}-a_{r,1} = 4*base-2 ---*/
    seq_t d2 = (seq_t)4 * base - (seq_t)2;

    /*--- 5. Compute 5^(c-1) -------------------------------------------------*/
    seq_t pow5 = (seq_t)1;
    for (idx_t i = 1; i < c; ++i)
        pow5 *= (seq_t)5;

    /*--- 6. Final value -----------------------------------------------------*/
    /* (pow5 - 1) is divisible by 4 for c ≥ 2, so integer division is exact */
    seq_t inc = d2 * (pow5 - (seq_t)1) / (seq_t)4;
    return base + inc;
}
