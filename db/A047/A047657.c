/*
 * A047657  Sextuple factorial numbers: a(n) = Product_{k=0..n-1} (6*k+2).
 * https://oeis.org/A047657
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) * (6*n - 4), a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = product_{k=0}^{n-1} (6*k + 2) */

seq_t a(idx_t n)
{
    /* For n == 0 the product is empty, defined to be 1. */
    if (n == 0)
        return (seq_t)1;

    seq_t result = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
    {
        /* Factor for term i (corresponds to k = i-1):
           factor = 6*(i-1) + 2 = 6*i - 4 */
        idx_t factor = 6 * i - 4;          /* fits in idx_t */
        result = result * (seq_t)factor;   /* multiplication with arbitrary-precision if needed */
    }
    return result;
}
