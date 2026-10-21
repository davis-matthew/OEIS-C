/*
 * A204820  a(n) = -4*a(n-1)*A001505(n-2), with a(1)=8.
 * https://oeis.org/A204820
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = -8 * (-1)^n * (4n-4)! / (n-1)!
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 4, i64 n <= 6, i128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent multiplicative formula:
   a(n) = (-8) * (-1)^n * ∏_{i=n}^{4n-4} i              */
seq_t a(idx_t n)
{
    /* For indices below the offset the result is irrelevant.
       Returning 0 for n==0 keeps the function well defined.     */
    if (n == 0)
        return 0;

    /* Compute the product ∏_{i=n}^{4n-4} i  */
    idx_t stop = 4 * n - 4;          /* 4n-4 is safe for n up to the intended limits   */
    seq_t prod = 1;
    for (idx_t i = n; i <= stop; ++i)
        prod *= (seq_t)i;

    /* Apply the sign factor: (-8) * (-1)^n  */
    seq_t sign = ((n & 1) ? (seq_t)8 : (seq_t)-8);

    return prod * sign;
}
