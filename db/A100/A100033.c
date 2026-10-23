/*
 * A100033  Bisection of A001700.
 * https://oeis.org/A100033
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(4*n+3, 2*n+2)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 3; a(n) = a(n-1) * 2*(4*n+1)*(4*n+3) / ((n+1)*(2*n+1)) */
static seq_t seq_gcd(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    seq_t result = (seq_t)3;          /* a(0) */
    if (n == (idx_t)0) return result;

    for (idx_t i = (idx_t)1; i <= n; ++i) {
        seq_t ii  = (seq_t)i;

        /* numerator = 2 * (4*i + 1) * (4*i + 3) */
        seq_t num = (seq_t)2 * ((seq_t)4 * ii + (seq_t)1) * ((seq_t)4 * ii + (seq_t)3);

        /* denominator = (i + 1) * (2*i + 1) */
        seq_t den = ((seq_t)ii + (seq_t)1) * ((seq_t)2 * ii + (seq_t)1);

        /* reduce with current result */
        seq_t g = seq_gcd(result, den);
        result /= g;
        den     /= g;

        /* reduce with numerator */
        g = seq_gcd(num, den);
        num /= g;
        den /= g;   /* should be 1, but keep safe */

        result = result * num;
        if (den != (seq_t)1) {
            result /= den;
        }
    }
    return result;
}
