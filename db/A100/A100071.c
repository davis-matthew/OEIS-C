/*
 * A100071  a(n) = n * binomial(n-1, floor((n-1)/2)) = n * max_{i=0..n}
 *          binomial(n-1, i).
 * https://oeis.org/A100071
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n * binomial(n-1, floor((n-1)/2))
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 30, u64 n <= 62, u128 n <= 125,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t my_gcd(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;
    }

    idx_t m = n - 1;           /* m = n - 1   */
    idx_t k = m / 2;           /* k = floor((n-1)/2) */

    seq_t result = 1;          /* intermediate binomial result */

    for (idx_t i = 1; i <= k; ++i) {
        seq_t num = (seq_t)(m - k + i);   /* numerator term */
        seq_t den = (seq_t)i;             /* denominator term */

        /* cancel common factors with the current result */
        seq_t g = my_gcd(result, den);
        result /= g;
        den /= g;

        /* cancel remaining factors between numerator and denominator */
        g = my_gcd(num, den);
        num /= g;
        den /= g;                 /* den should now be 1 */

        result *= num;             /* multiply the reduced numerator */
    }

    return (seq_t)n * result;      /* final value a(n) */
}
