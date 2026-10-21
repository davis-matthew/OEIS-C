/*
 * A062111  Upper-right triangle resulting from binomial transform calculation
 *          for nonnegative integers.
 * https://oeis.org/A062111
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 104, u32 n <= 434, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  $seq_t a(idx_t n)$
 *
 *  Sequence A062111 – “Upper‑right triangle resulting from binomial
 *  transform calculation for nonnegative integers”.
 *
 *  The sequence is obtained by listing the entries T(n,k) of the lower
 *  triangle of the table
 *
 *      T(n,k) = (n+k) * 2^(n-k-1) ,   0 ≤ k < n
 *      T(n,n) = n
 *
 *  for every n ≥ 0 and 0 ≤ k ≤ n.  The entries are read row by row:
 *  (0,0), (1,0),(1,1), (2,0),(2,1),(2,2), … .
 *
 *  The implementation finds the row `r` that contains the index `n`,
 *  computes the column `c = n - r(r+1)/2`, and evaluates T(r,c)
 *  using only integer arithmetic.
 *
 *  All arithmetic is performed in the caller’s integer types (`idx_t`
 *  and `seq_t`).  The algorithm is O(√n) in time and O(1) memory,
 *  easily fast enough for n ≲ 10 000 on any build.
 */

seq_t a(idx_t n)
{
    /* locate the row r :  r(r+1)/2 ≤ n < (r+1)(r+2)/2  */
    idx_t r = 0;
    while ((r * (r + 1)) / 2 <= n) {
        ++r;
    }
    --r;                           /* r now satisfies start ≤ n */

    idx_t start = r * (r + 1) / 2;  /* first index of row r   */
    idx_t c = n - start;            /* column within that row */

    /* diagonal entry */
    if (r == c) {
        return (seq_t)r;
    }

    /* non‑diagonal:  T(r,c) = (r+c) * 2^(r-c-1)  (c < r) */
    seq_t coeff = (seq_t)(r + c);
    idx_t exp   = r - c - 1;        /* non‑negative */

    seq_t result = coeff;
    while (exp--) {
        result <<= 1;               /* shift by 1 each iteration – safe for unsigned types */
    }
    return result;
}
