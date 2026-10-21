/*
 * A215862  Number of simple labeled graphs on n+2 nodes with exactly n
 *          connected components that are trees or cycles.
 * https://oeis.org/A215862
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(n+2,3)*(3*n+13)/4 (OEIS A215862)
 * Range:     u8 n <= 5, u16 n <= 25, u32 n <= 308, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;

    /* binomial coefficient C(n+2,3) = (n+2)*(n+1)*n / 6
       The product of three consecutive numbers is always divisible by 6. */
    seq_t binom = ((nn + 2) * (nn + 1) * nn) / 6;

    /* numerator (3*n + 13) */
    seq_t numer = nn * 3 + 13;

    /* final result: (C(n+2,3) * numer) / 4
       The product is always divisible by 4, so integer division is exact. */
    return (binom * numer) / 4;
}
