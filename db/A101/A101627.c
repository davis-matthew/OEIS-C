/*
 * A101627  Numerator of partial sums of a certain series.
 * https://oeis.org/A101627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = numerator of 24 * Σ_{k=1..n} 1/((3k-1)(3k)(3k+1))
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Local GCD for seq_t.  Use Euclidean algorithm.  */
static seq_t local_gcd(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n == 0)   /* Outside the defined range – return 0 for safety */
        return (seq_t)0;

    seq_t num = (seq_t)0;   /* current numerator   */
    seq_t den = (seq_t)1;   /* current denominator */

    for (idx_t k = 1; k <= n; ++k) {
        /* 3k, (3k-1), (3k+1) as seq_t values */
        seq_t three_k   = (seq_t)(3 * k);
        seq_t denom_k   = (three_k - (seq_t)1) * three_k * (three_k + (seq_t)1); /* (3k-1)(3k)(3k+1) */
        seq_t term_num  = (seq_t)24;                                            /* 24 */

        /* Reduce the product of denominators before multiplication to keep sizes small */
        seq_t g = local_gcd(den, denom_k);
        seq_t den_div_g = den / g;
        seq_t denom_div_g = denom_k / g;

        /* New denominator = lcm(den, denom_k) = den_div_g * denom_k */
        seq_t new_den = den_div_g * denom_k;

        /* New numerator = num * (denom_k/g) + term_num * (den/g) */
        seq_t new_num = num * denom_div_g + term_num * den_div_g;

        /* Final reduction of the new rational number */
        seq_t g2 = local_gcd(new_num, new_den);
        new_num /= g2;
        new_den /= g2;

        num = new_num;
        den = new_den;
    }

    return num;   /* Return the numerator of the reduced fraction s(n) */
}
