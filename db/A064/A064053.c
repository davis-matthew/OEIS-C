/*
 * A064053  Auxiliary sequence gamma(n) used to compute coefficients in series
 *          expansion of the mock theta function f(q) via A(n) = Sum_{r=0..n}
 *          p(r)*gamma(n-r), with p(r) the partition function A000041.
 * https://oeis.org/A064053
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1; for n >= 1,
 * Range:     i8 n <= 76, i16 n <= 5000, i32 n <= 5000, i64 n <= 5000,
 *            i128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = Σ_{k≥1} 4·(-1)^{k+m} where m = (n - k·(3k+1)/2)/k,
   provided the division is integral and m ≥ 0. */
/* This follows directly from the generating function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 1;               /* constant term of the G.f. */

    seq_t result = 0;            /* accumulator for the coefficient */

    for (idx_t k = 1; ; ++k)
    {
        /* exponent = k * (3k + 1) / 2, computed as (k*(3k+1))/2 */
        seq_t prod = (seq_t)k * ((seq_t)3 * k + 1);
        seq_t exponent = prod / 2;

        if (exponent > n)
            break;               /* further k's give larger exponents */

        seq_t diff = (seq_t)n - exponent;
        if (diff % k != 0)
            continue;            /* m is not an integer */

        seq_t m = diff / k;
        /* sign: (-1)^(k+m) */
        int sign = ((k + m) & 1) ? -1 : 1;
        result += sign * (seq_t)4;
    }

    return result;
}
