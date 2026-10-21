/*
 * A229039  G.f.: Sum_{n>=0} (n+2)^n * x^n / (1 + (n+2)*x)^n.
 * https://oeis.org/A229039
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+5) * n! / 2 for n>0 with a(0)=1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base case for the offset 0. */
    if (n == 0)
        return (seq_t)1;

    /* Compute n! iteratively. */
    seq_t fact = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i)
        fact = fact * (seq_t)i;      /* multiplication by small integer */

    /* Multiply by (n+5) and divide by 2. */
    seq_t result = fact * (seq_t)(n + 5);
    result /= (seq_t)2;              /* exact integer division */

    return result;
}
