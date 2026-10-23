/*
 * A173038  a(n) = (1/4)*(n^2 - 5*n + 2)*(n-2)! + 1.
 * https://oeis.org/A173038
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/4)*(n^2 - 5*n + 2)*(n-2)! + 1, for n ≥ 2 The first
 *            three terms (n=2,3,4) are zero because the polynomial part is
 *            negative. For n ≥ 5 the expression is always integer
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Small n do not fit the formula; they are zero by definition. */
    if (n < 5) {
        return (seq_t)0;
    }

    /* Polynomial part: n^2 - 5*n + 2.  Use seq_t arithmetic to avoid
       overflow on the intermediate unsigned multiplication. */
    seq_t poly = (seq_t)n * (seq_t)n - (seq_t)5 * (seq_t)n + (seq_t)2;

    /* Factorial (n-2)!  (for n ≥ 5, (n-2) ≥ 3, so the loop runs). */
    seq_t fact = 1;
    for (idx_t i = 2; i <= n - 2; ++i) {
        fact *= (seq_t)i;
    }

    /* Compute gcd(poly,4).  Since 4 is tiny we can test directly. */
    seq_t g = ((poly & (seq_t)1) == 0) ? ((poly & (seq_t)3) == 0 ? (seq_t)4
                                                               : (seq_t)2)
                                      : (seq_t)1;

    /* divisor = 4 / g  (1, 2, or 4).  All arithmetic is on seq_t. */
    seq_t divisor = ((seq_t)4) / g;

    /* Reduce the division by 4 using the factor g:  (poly/g) * ((n-2)! / divisor) */
    seq_t fact_div = fact / divisor;
    seq_t t = poly / g;
    seq_t prod = fact_div * t;

    /* Final addition of 1. */
    return prod + (seq_t)1;
}
