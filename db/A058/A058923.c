/*
 * A058923  a(n) = binomial(n,0) - binomial(n,2) + binomial(n,4).
 * https://oeis.org/A058923
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (n-2)*(n³-4n²-9n-12)/24
 * Range:     i8 n <= 6, i16 n <= 15, i32 n <= 200, i64 n <= 200,
 *            i128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent combinatorial definition: a(n) = binomial(n,0) - binomial(n,2) + binomial(n,4)  */

seq_t a(idx_t n)
{
    seq_t ns = (seq_t)n;

    /* Compute n² and n³ using only seq_t arithmetic */
    seq_t n2 = ns * ns;        /* n² */
    seq_t n3 = n2 * ns;        /* n³ */

    /* temp = n³ - 4n² - 9n - 12 */
    seq_t temp = n3 - 4 * n2 - 9 * ns - 12;

    /* coeff = n - 2 */
    seq_t coeff = ns - 2;

    /* product = (n-2) * (n³ - 4n² - 9n - 12) */
    seq_t prod = coeff * temp;

    /* Final division by 24 (exact) */
    prod /= 24;

    return prod;
}
