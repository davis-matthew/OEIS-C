/*
 * A180056  The number of permutations of {1,2,...,2n} with n ascents.
 * https://oeis.org/A180056
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{j=0..n} (-1)^j * binom(2n+1, j) * (n-j+1)^(2n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helper: fast exponentiation of seq_t to an unsigned integer exponent */
static seq_t pow_seq_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    seq_t power  = base;
    while (exp > 0) {
        if (exp & 1)
            result *= power;
        power *= power;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    idx_t m = 2 * n;              /* exponent and part of binomial */
    seq_t sum = (seq_t)0;

    /* bin = C(2n+1, j), starting with j = 0 */
    seq_t bin = (seq_t)1;

    for (idx_t j = 0; j <= n; ++j) {
        seq_t base = (seq_t)(n - j + 1);     /* base = n-j+1          */
        seq_t power = pow_seq_seq(base, m);  /* base^(2n)              */
        seq_t term  = bin * power;           /* bin * power            */

        if (j & 1)
            sum -= term;
        else
            sum += term;

        /* update binomial for next j (if any) */
        if (j < n) {
            seq_t numerator = (seq_t)(m + 1 - j);          /* (2n+1 - j) */
            seq_t denominator = (seq_t)(j + 1);           /* (j+1)     */
            bin = bin * numerator / denominator;
        }
    }

    return sum;
}
