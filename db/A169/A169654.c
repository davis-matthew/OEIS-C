/*
 * A169654  Triangle T(n, k) = A169643(n, k) - A169653(n, 1) + 1, read by
 *          rows.
 * https://oeis.org/A169654
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl, easy, less
 * Formula:   a(n) computed from the triangle T(n,k) defined by T(n,k) =
 *            (-1)^n * ((n!/k!*C(n-1,k-1) + n!/(n-k+1)!*C(n-1,n-k)) - n! - 1)
 *            + 1 where C is the standard binomial coefficient and n! is the
 *            factorial. The sequence a(n) is obtained by reading the triangle
 *            row by row (n=1 is the first element of the fi...
 * Range:     i8 n <= 11, i16 n <= 29, i32 n <= 67, i64 n <= 191,
 *            i128 n <= 530, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helper function: product of integers from lo to hi inclusive.
   If lo > hi the product is 1. */
static seq_t prod_range(idx_t lo, idx_t hi) {
    seq_t p = 1;
    for (idx_t i = lo; i <= hi; ++i) {
        p *= (seq_t)i;
    }
    return p;
}

/* Helper function: binomial coefficient C(n,k) with n,k being
   idx_t values. Uses multiplicative formula to avoid overflow. */
static seq_t binomial_idx(idx_t n, idx_t k) {
    if (k > n) return 0;
    if (k > n - k) k = n - k;          /* use smaller k */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - i + 1) / (seq_t)i;
    }
    return res;
}

/* Main function: returns the nth term of OEIS sequence A169654 */
seq_t a(idx_t n) {
    /* --- find the row r such that r*(r+1)/2 >= n --- */
    idx_t r = 1;
    while (((seq_t)r * (r + 1) / 2) < (seq_t)n) {
        ++r;
    }
    /* index (row r) starts at term number prev + 1 where
       prev = (r-1)*r/2 */
    idx_t prev = (r - 1) * r / 2;
    idx_t c = n - prev;          /* column within the row, 1-based */

    /* sign factor (-1)^r */
    seq_t s = (r % 2 == 0) ? (seq_t)1 : (seq_t)-1;

    /* factorial r! */
    seq_t fact = 1;
    for (idx_t i = 2; i <= r; ++i) {
        fact *= (seq_t)i;
    }

    /* products needed for t(r,c) and t(r,r-c+1) */
    seq_t prod1 = (c == r) ? 1 : prod_range(c + 1, r);
    idx_t lo2 = r - c + 2;
    seq_t prod2 = (lo2 > r) ? 1 : prod_range(lo2, r);

    /* binomial coefficient C(r-1, c-1) */
    seq_t binom = binomial_idx(r - 1, c - 1);

    /* compute T(r,c) = s * ((prod1+prod2)*binom - fact - 1) + 1 */
    seq_t temp = (prod1 + prod2) * binom - fact - 1;
    seq_t result = s * temp + 1;
    return result;
}
