/*
 * A135765  Distribute the odd numbers in columns based on the occurrence of
 *          "3" in each prime factorization; square array A(row, col) =
 *          3^(row-1) * A007310(col), read by antidiagonals A(1,1), A(1,2),
 *          A(2,1), A(1,3), A(2,2), A(3,1), ...
 * https://oeis.org/A135765
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = 3^(row‑1) * A007310(col) where (row,col) are the
 *            antidiagonal coordinates of n (row, col start at 1). A007310(c)
 *            is the sequence of odd numbers that are not divisible by 3: if c
 *            is odd -> 3*c – 2 if c is even -> 3*c – 1 Hence a(n) = 3^(row‑1)
 *            * ( 3*c – (c&1?2:1) )
 * Range:     u8 n <= 19, u16 n <= 64, u32 n <= 229, u64 n <= 859,
 *            u128 n <= 3400, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the diagonal k containing n   (k >= 1)   */
    idx_t k = 1;
    idx_t cumulative = 1;          /* k*(k+1)/2 for the current k   */
    while (cumulative < n) {
        ++k;
        cumulative += k;           /* cumulative = k*(k+1)/2        */
    }

    /* Position inside this diagonal (0‑based)   */
    idx_t prev = cumulative - k;  /* (k-1)*k/2                      */
    idx_t offset = n - prev - 1;  /* 0 ≤ offset < k                 */
    idx_t r = offset + 1;         /* row number (1‑based)            */
    idx_t c = k - offset;         /* column number (1‑based)         */

    /* Compute A007310(c): odd numbers not divisible by 3     */
    idx_t odd_tmp = 3 * c;                        /* 3*c fits in 64‑bit for all n in tests   */
    seq_t odd = (c & 1) ? (seq_t)odd_tmp - 2
                        : (seq_t)odd_tmp - 1;

    /* Compute 3^(r-1)                                            */
    seq_t pow3 = 1;
    for (idx_t i = 0; i < r - 1; ++i) {
        pow3 *= 3;
    }

    /* Return the product                                         */
    return pow3 * odd;
}
