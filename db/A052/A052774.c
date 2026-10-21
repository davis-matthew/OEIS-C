/*
 * A052774  a(n) = (4*n+1)^(n-1).
 * https://oeis.org/A052774
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n + 1)^(n-1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* Compute the base 4*n+1 in seq_t */
    seq_t base = (seq_t)1 + ((seq_t)4 * (seq_t)n);

    /* Exponentiation by repeated multiplication.
       For n=1 the loop does not execute, yielding 1 as required. */
    seq_t result = (seq_t)1;
    for (idx_t i = 1; i <= n - 1; ++i)
    {
        result *= base;
    }

    return result;
}
