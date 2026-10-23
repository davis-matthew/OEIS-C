/*
 * A009178  Expansion of cosh(x)*cosh(log(1+x)).
 * https://oeis.org/A009178
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient of x^n in cosh(x) * cosh(log(1+x))
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 12, i64 n <= 20, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A009178 – expansion of cosh(x) * cosh(log(1+x)) */

seq_t a(idx_t n)
{
    /* Handle the first two terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;

    /* Compute n! */
    seq_t fact = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i)
        fact *= (seq_t)i;

    /* Sum the terms  n! / (2k)!  for k = 0 .. floor((n-2)/2)  */
    seq_t current = fact;
    seq_t sum = (seq_t)0;

    for (idx_t k = 0; ; ++k)
    {
        sum += current;

        /* Stop when 2k+2 > n-2 (i.e. we would exceed the allowed kmax) */
        idx_t next = 2 * k + 2;
        if (next > n - 2U)
            break;

        /* current = current / ((2k+2)*(2k+1))  which is exact */
        seq_t denom = (seq_t)next * (seq_t)(next - 1U);
        current /= denom;
    }

    /* Assemble the final result:
       a(n) = (n even ? 1 : 0) + ((-1)^n) * (sum / 2)   */
    seq_t delta = (n & 1U) ? (seq_t)0 : (seq_t)1;
    seq_t half = sum / (seq_t)2U;
    seq_t sign = (n & 1U) ? (seq_t)(-1) : (seq_t)1;

    return delta + half * sign;
}
