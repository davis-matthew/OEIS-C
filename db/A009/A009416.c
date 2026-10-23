/*
 * A009416  Expansion of e.g.f. log(1+x)*cosh(x).
 * https://oeis.org/A009416
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   * a(n) = -(-1)^n * sum_{k=0..floor((n-1)/2)}
 *            binomial(n,2*k)*(n-2*k-1)! * a(0) = 0 * The implementation
 *            evaluates the sum by updating the binomial coefficient * and the
 *            trailing factorial in O(n) time, keeping all computations in the
 *            * seq_t type
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 13, i64 n <= 21, i128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u)
        return 0;

    /* factorial of (n-1)! */
    seq_t fact = 1;
    for (idx_t i = 2; i <= n - 1; ++i)
        fact *= (seq_t)i;

    /* binomial coefficient binomial(n,0) = 1 */
    seq_t bin = 1;

    /* initial term k = 0 */
    seq_t sum = bin * fact;

    idx_t kmax = (n - 1) / 2;
    for (idx_t k = 1; k <= kmax; ++k)
    {
        /* Update binomial: bin = bin * (n-2k+2)*(n-2k+1) / ((2k)*(2k-1)) */
        seq_t num1 = (seq_t)(n - 2 * k + 2);
        seq_t num2 = (seq_t)(n - 2 * k + 1);
        seq_t den1 = (seq_t)(2 * k);
        seq_t den2 = (seq_t)(2 * k - 1);

        bin *= num1;
        bin *= num2;
        bin /= den1;
        bin /= den2;

        /* Update factorial: fact = fact / ((n-2k+1)*(n-2k)) */
        seq_t f_den1 = (seq_t)(n - 2 * k + 1);
        seq_t f_den2 = (seq_t)(n - 2 * k);
        fact /= f_den1;
        fact /= f_den2;

        sum += bin * fact;
    }

    /* Apply the global sign: a(n) = (-1)^(n+1) * sum */
    if ((n & 1u) == 0)
        return -sum;
    else
        return sum;
}
