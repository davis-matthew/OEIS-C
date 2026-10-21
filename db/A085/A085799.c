/*
 * A085799  Determinant of the symmetric n X n matrix A defined by A[i,j] =
 *          abs(i^2 - j^2) for 1 <= i,j <= n.
 * https://oeis.org/A085799
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0)=1, a(1)=0, and for n>=2 a(n) = (-1)^(n-1) * (n+1)/2 *
 *            (2n-1)! / (n-2)! (determinant of |i^2-j^2|)
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 7, i64 n <= 13, i128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset terms */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    /* n >= 2 */
    seq_t prod = (seq_t)1;
    idx_t k;
    /* product_{k=n-1}^{2n-1} k = (2n-1)! / (n-2)! */
    for (k = n - 1; k <= 2 * n - 1; ++k)
        prod *= (seq_t)k;

    /* Multiply by (n+1)/2, taking care of the division by 2 */
    if ((n + 1) % 2 == 0) {
        prod *= (seq_t)((n + 1) / 2);
    } else {
        prod = prod / (seq_t)2 * (seq_t)(n + 1);
    }

    /* Apply the sign (-1)^(n-1) */
    if ((n - 1) % 2 == 1)
        return -prod;
    else
        return prod;
}
