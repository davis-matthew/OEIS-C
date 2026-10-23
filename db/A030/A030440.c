/*
 * A030440  Values of Newton-Gregory forward interpolating polynomial
 *          (1/3)*(n-1)*(2*n+3)*(2*n-1).
 * https://oeis.org/A030440
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1)*(2n+3)*(2n-1)/3 (for n ≥ 0)
 * Range:     u8 n <= 5, u16 n <= 36, u32 n <= 1476, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the small indices where subtraction would underflow
       the unsigned seq_t type. */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)0;

    seq_t x = (seq_t)n;     /* cast once to the one arithmetic type used */

    /* Compute the three linear factors */
    seq_t f1 = x - (seq_t)1;         /* n-1 */
    seq_t f2 = x * (seq_t)2 + (seq_t)3; /* 2n+3 */
    seq_t f3 = x * (seq_t)2 - (seq_t)1; /* 2n-1 */

    /* Divide one of the factors by 3.  Because the product is
       always divisible by 3, at least one factor is.
       We prefer to divide the first that satisfies the test to
       keep intermediate values as small as possible. */
    if (f1 % (seq_t)3 == (seq_t)0)
        f1 /= (seq_t)3;
    else if (f2 % (seq_t)3 == (seq_t)0)
        f2 /= (seq_t)3;
    else
        f3 /= (seq_t)3;

    /* The product now equals a(n) exactly. */
    return f1 * f2 * f3;
}
