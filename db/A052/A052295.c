/*
 * A052295  a(n) = (n*(n+1)/2)!.
 * https://oeis.org/A052295
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n*(n+1)/2)!
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute the n-th triangular number */
    seq_t tri = (seq_t)n * ((seq_t)n + 1) / 2;

    /* factorial of that triangular number */
    seq_t res = 1;
    for (seq_t i = 2; i <= tri; ++i)
    {
        res *= i;
    }
    return res;
}
