/*
 * A091036  Sixth column (k=7) of array A090438 ((4,2)-Stirling2) divided by
 *          48 = 4!*2.
 * https://oeis.org/A091036
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 10, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: (2*n-7)*(n-4)*a(n) = 2*n*(n-1)*(2*n-1)*(2*n-3)*a(n-1),  a(4)=1 */
seq_t a(idx_t n)
{
    if (n < 4)
        return 0;                 /* undefined for n<4, but not used */

    seq_t result = 1;              /* a(4) = 1 */

    for (idx_t i = 5; i <= n; ++i)
    {
        /* compute numerator = 2*i*(i-1)*(2*i-1)*(2*i-3) */
        seq_t numerator =
            (seq_t)2 * (seq_t)i * (seq_t)(i - 1) * (seq_t)(2 * i - 1) * (seq_t)(2 * i - 3);

        /* compute denominator = (2*i-7)*(i-4) */
        seq_t denominator = (seq_t)(2 * i - 7) * (seq_t)(i - 4);

        /* a(i) = a(i-1) * numerator / denominator  */
        result = result * numerator / denominator;
    }

    return result;
}
