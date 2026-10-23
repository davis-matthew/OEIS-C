/*
 * A134225  A007436 + A134082 - A000012 as infinite lower triangular matrices;
 *          where A000012 = (1; 1,1; 1,1,1; ...).
 * https://oeis.org/A134225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A134225: triangle from A007436 + A134082 - A000012.
 * Element T(r,j) for row r ≥ 1 and column 1 ≤ j ≤ r is
 *   1               if j = r,
 *   2*r - 1         if j = r-1 (only if r ≥ 2),
 *   r - j           otherwise (i.e. 1 ≤ j ≤ r-2).
 * a(n) is the element at overall index n in row‑major order.
 * The formula for a(n) is therefore:
 *   Let r be the unique integer with r(r+1)/2 ≥ n.
 *   Let j = n - r(r-1)/2  (1 ≤ j ≤ r).
 *   return (j == r) ? 1 :
 *          (j == r-1) ? 2*r - 1 :
 *                       r - j;
 */
seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 ≥ n. */
    idx_t r = 1;
    /* Small n (≤10 000) means r is ≤ 150, so a simple loop is fine. */
    while ((r * (r + 1)) / 2 < n)
        r++;

    /* Column index within the row. */
    idx_t j = n - (r * (r - 1)) / 2;

    if (j == r)
        return (seq_t)1;
    else if (j == r - 1)
        return (seq_t)(2 * r - 1);
    else
        return (seq_t)(r - j);
}
