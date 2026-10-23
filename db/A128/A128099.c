/*
 * A128099  Triangle read by rows: T(n,k) is the number of ways to tile a 3 X
 *          n rectangle with k pieces of 2 X 2 tiles and 3n-4k pieces of 1 X 1
 *          tiles (0 <= k <= floor(n/2)).
 * https://oeis.org/A128099
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = 2^k * binomial(r - k, k) where (r,k) is the position of
 *            the nth element in the triangle read row‑major. The triangle is
 *            defined by T(r,k) = 2^k * binomial(r - k, k) (0 ≤ k ≤ ⌊r/2⌋).
 *            For a given index n we find the smallest r such that
 *            sum_{i=0}^{r} (⌊i/2⌋+1) > n and then k = n - sum_{i=0}^...
 * Range:     u8 n <= 32, u16 n <= 105, u32 n <= 334, u64 n <= 1210,
 *            u128 n <= 2549, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow2_idx(idx_t k) {
    seq_t res = 1;
    while (k-- > 0) res *= 2;
    return res;
}

static seq_t binomial_idx(idx_t n, idx_t k) {
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;          /* use symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - k + i);  /* multiply by the numerator term */
        res = res / i;            /* divide by the denominator term */
    }
    return res;
}

seq_t a(idx_t n) {
    /* find the row r where the nth element lies */
    idx_t cum = 0;   /* cumulative count of elements so far */
    idx_t r = 0;     /* current row index */

    while (1) {
        idx_t len = (r / 2) + 1;              /* length of the current row */
        idx_t next = cum + len;
        if (n < next) break;                 /* n is inside this row */
        cum = next;
        ++r;
    }

    /* compute the position k within row r */
    idx_t k = n - cum;   /* 0‑based index within the row */

    /* compute the triangle entry T(r,k) = 2^k * binomial(r-k, k) */
    seq_t pow2_k   = pow2_idx(k);
    seq_t binom_val = binomial_idx(r - k, k);
    return pow2_k * binom_val;
}
