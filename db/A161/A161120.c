/*
 * A161120  Number of cycles with entries of opposite parities in all
 *          fixed-point-free involutions of {1,2,...,2n}.
 * https://oeis.org/A161120
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^2 * (2n-3)!! (double factorial of odd numbers)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n = 0 or 1 the formula holds trivially. */
    if (n <= 1)
        return (seq_t)n;

    /* Compute the odd double factorial (2n-3)!! */
    idx_t upper = 2 * n - 3;   /* safe for n >= 2 */
    seq_t fact = 1;
    /* Start at 3 because 1*... is just 1 */
    for (idx_t k = 3; k <= upper; k += 2)
        fact *= (seq_t)k;

    /* Multiply by n^2 */
    seq_t nsq = (seq_t)n * (seq_t)n;
    return nsq * fact;
}
