/*
 * A096313  a(n) = determinant of n X n matrix m(i,j) = Product_{k=1..i} k+j.
 * https://oeis.org/A096313
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)! * ∏_{k=1}^{n-1} k! (n>1) a(0) = 1; a(1) = 2
 *            Computed via incremental factorials to keep intermediate values
 *            minimal
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    seq_t result = (seq_t)1;      /* product of factorials 1! … (n-1)!  */
    seq_t fact   = (seq_t)1;      /* running factorial */

    for (idx_t i = 1; i < n; ++i) {
        fact *= (seq_t)i;         /* fact = i!  (i runs 1..n‑1)   */
        result *= fact;           /* accumulate ∏ k!  */
    }

    /* multiply by (n+1)!  (fact currently (n-1)!) */
    fact *= (seq_t)n;              /* now fact = n!          */
    fact *= (seq_t)(n + 1);        /* now fact = (n+1)!      */
    result *= fact;                /* final product           */

    return result;
}
