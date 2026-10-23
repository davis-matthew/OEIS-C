/*
 * A180178  Triangle read by rows: T(n,k) is the number of compositions of n
 *          without 3's and having k parts; 1 <= k <= n.
 * https://oeis.org/A180178
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = Sum_{j=ceil((3k-n)/2)}^k (-1)^(k-j) * C(k,j) *
 *            C(n-3k+3j-1, j-1); a(n) is the nth element of the flattened
 *            triangle read by rows
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* binomial coefficient for non‑negative arguments, computed multiplicatively. */
static seq_t binom_uint(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;          /* take advantage of symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i) / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Find the row r (sum) and column k (part count) that correspond to
       the nth flattened element.  n ≥ 1. */
    idx_t r = 1;
    idx_t prev_sum = 0;                /* sum of 1..(r-1) */
    while (prev_sum + r < n) {
        prev_sum += r;
        ++r;
    }
    idx_t k = n - prev_sum;            /* 1 ≤ k ≤ r */

    /* Determine the lower bound for j in the inclusion–exclusion sum. */
    idx_t j_min;
    int64_t tmp = 3 * (int64_t)k - (int64_t)r;
    if (tmp <= 0)
        j_min = 0;
    else
        j_min = (idx_t)((tmp + 1) / 2); /* ceil division for positive tmp */

    seq_t sum_pos = 0, sum_neg = 0;     /* accumulate positive/negative terms */

    for (idx_t j = j_min; j <= k; ++j) {
        seq_t binCkj = binom_uint(k, j);
        int64_t upper_ll = (int64_t)r - 3LL * k + 3LL * j - 1;
        seq_t term;

        if (j == 0) {
            /* Special case: C(-1,-1)=1, all other lower=-1 give 0. */
            if (upper_ll == -1)
                term = 1;               /* C(k,0)=1 and C(-1,-1)=1 */
            else
                continue;               /* term is zero */
        } else {
            if (upper_ll < 0)           /* binomial is zero for negative upper */
                continue;
            /* lower = j-1 ≥ 0 */
            term = binom_uint((idx_t)upper_ll, j - 1);
        }

        term *= binCkj;                 /* (-1)^(k-j) will be handled via sign */

        if (( (k - j) & 1 ) == 0)      /* even exponent → positive sign */
            sum_pos += term;
        else                           /* odd exponent → negative sign */
            sum_neg += term;
    }

    /* The final result is guaranteed non‑negative. */
    return sum_pos - sum_neg;
}
