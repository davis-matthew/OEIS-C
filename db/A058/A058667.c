/*
 * A058667  2^(n-2)*n*(n+2)!/3.
 * https://oeis.org/A058667
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n-2) * n * (n+2)! / 3
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)                 /* a(0)=0, a(1)=1 */
        return (seq_t)n;

    /* Compute n * (n+2)! */
    seq_t prod = (seq_t)n;
    idx_t upper = n + 2;
    for (idx_t i = 1; i <= upper; ++i)
        prod *= (seq_t)i;

    prod /= (seq_t)3;           /* integer division – divisible by 3 */

    /* Multiply by 2^(n-2) using repeated doubling
       (avoids undefined behaviour of shifting more than width).    */
    for (idx_t i = 0; i < n - 2; ++i)
        prod *= (seq_t)2;

    return prod;
}
