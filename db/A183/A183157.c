/*
 * A183157  Triangle read by rows: T(n,k) is the number of partial isometries
 *          of an n-chain of height k (height of alpha = |Im(alpha)|).
 * https://oeis.org/A183157
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(r,k) where r is the row satisfying r(r+1)/2 <= n <
 *            (r+1)(r+2)/2 and k = n - r(r+1)/2. T(r,0)=1, T(r,1)=r^2, T(r,k)=
 *            2*(2*r-k+1)*C(r,k)/(k+1) for k>1
 * Range:     u8 n <= 37, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute integer GCD of two seq_t values */
static inline seq_t gcd_seq_t(seq_t a, seq_t b) {
    while (b != 0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Compute binomial coefficient C(n,k) with minimal overflow */
static inline seq_t binom_seq(idx_t n, idx_t k) {
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t num = n - k + i;   /* numerator term   */
        seq_t den = i;           /* denominator term */

        /* cancel common factors between num and den */
        seq_t g = gcd_seq_t(num, den);
        num /= g;
        den /= g;

        /* cancel common factors between res and den */
        g = gcd_seq_t(res, den);
        res /= g;
        den /= g;

        /* den should now be 1; multiply res by remaining numerator */
        res *= num;
    }
    return res;
}

/* Compute T(n,k) as defined for the triangle */
static inline seq_t partial_iso(idx_t n, idx_t k) {
    if (k == 0) return 1;
    if (k == 1) return (seq_t)n * (seq_t)n;

    seq_t c = binom_seq(n, k);

    /* factor = 2 * (2*n - k + 1) / (k + 1) */
    seq_t factor_num = 2 * ((seq_t)2 * n - k + 1);
    seq_t factor_den = k + 1;

    /* reduce c and denominator first */
    seq_t g = gcd_seq_t(c, factor_den);
    c /= g;
    factor_den /= g;

    /* reduce remaining numerator with denominator */
    g = gcd_seq_t(factor_num, factor_den);
    factor_num /= g;
    factor_den /= g;

    /* final multiplication */
    seq_t result = c * factor_num;
    if (factor_den != 1) result /= factor_den;
    return result;
}

/* Main sequence function as required by the build */
seq_t a(idx_t n) {
    /* find the row r with r(r+1)/2 <= n < (r+1)(r+2)/2 */
    idx_t r = 0;
    /* use unsigned __int128 to avoid overflow in the product */
    while ((unsigned __int128)(r + 1) * (r + 2) / 2 <= (unsigned __int128)n) {
        ++r;
    }

    idx_t offset = r * (r + 1) / 2;
    idx_t k = n - offset;
    return partial_iso(r, k);
}
