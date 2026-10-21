/*
 * A216414  a(n) = (-1)^(n-3)*binomial(n,3) - 1.
 * https://oeis.org/A216414
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (-1)^(n-3) * binomial(n,3) - 1
 * Range:     i8 n <= 10, i16 n <= 59, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n >= 3 the sign is +1 for odd n and -1 for even n. */
    seq_t sign = (n & 1) ? (seq_t)1 : (seq_t)-1;

    /* Compute binomial(n,3) = n*(n-1)*(n-2)/6 in a safe order. */
    seq_t nn = (seq_t)n;
    seq_t bin = nn;                /* bin = n */
    bin = bin * (nn - 1) / 2;      /* bin = n*(n-1)/2, always integer */
    bin = bin * (nn - 2) / 3;      /* bin = n*(n-1)*(n-2)/6, always integer */

    /* Final formula: sign * binomial(n,3) - 1 */
    return sign * bin - 1;
}
