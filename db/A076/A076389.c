/*
 * A076389  Sum of squares of numbers that cannot be written as t*n + u*(n+1)
 *          for nonnegative integers t,u.
 * https://oeis.org/A076389
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^2*(n^2-1)*(n^2-n-1)/12 (offset 1)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 61, u64 n <= 2459,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t seq_gcd(seq_t a, seq_t b)
{
    while (b != 0)
    {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;

    seq_t N   = (seq_t)n;          /* n as seq_t */
    seq_t n2  = N * N;             /* n^2 */
    seq_t t1  = n2 - 1;            /* n^2-1 = (n-1)*(n+1) */
    seq_t t2  = n2 - N - 1;        /* n^2-n-1 */

    /* Reduce the denominator 12 with the numerator factors as far as possible */
    seq_t terms[3] = {n2, t1, t2};
    seq_t denom    = 12;

    for (int i = 0; i < 3 && denom > 1; ++i)
    {
        seq_t g = seq_gcd(terms[i], denom);
        if (g > 1)
        {
            terms[i] /= g;
            denom   /= g;
        }
    }

    /* Multiply the remaining factors, dividing by the remaining denom after each step */
    seq_t result = terms[0];
    for (int i = 1; i < 3; ++i)
    {
        /* divide result by any common factor with the remaining denominator */
        seq_t g = seq_gcd(result, denom);
        if (g > 1)
        {
            result /= g;
            denom  /= g;
        }
        result *= terms[i];
    }

    /* final division by any remaining part of the denominator */
    if (denom > 1)
        result /= denom;

    return result;
}
