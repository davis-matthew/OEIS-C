/*
 * A177040  Irregular triangle t(n,m) = binomial(m+1,n-m) read by rows
 *          floor((n+1)/2) <= m <= n.
 * https://oeis.org/A177040
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A177040: irregular triangle t(n,m)=C(m+1,n-m) read by rows
   floor((n+1)/2) <= m <= n.  The function returns the n‑th term (offset 0). */

/* Integer square root: floor(sqrt(v)) for unsigned 64‑bit numbers. */
static inline idx_t isqrt_u64(idx_t v)
{
    idx_t low = 0, high = v;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid == 0) {
            if (v == 0) return 0;
            low = 1;
            continue;
        }
        idx_t div = v / mid;
        if (mid > div) {
            high = mid - 1;
        } else if (mid < div) {
            low = mid + 1;
        } else {
            return mid;
        }
    }
    return high;
}

/* Multiplicative binomial coefficient: C(n,k) for unsigned 64‑bit arguments. */
static inline seq_t binom_idx(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k == 0 || k == n) return 1;
    if (k > n - k) k = n - k;
    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i) {
        result = result * (seq_t)(n - k + i);
        result = result / (seq_t)i;
    }
    return result;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Linear index of the term. */
    idx_t k = n;

    /* Determine the row number n_row from k using the known cumulative
       counts: S_{2p-1} = p(p+1) and S_{2p} = (p+1)^2. */
    idx_t p = isqrt_u64(k);          /* p = floor(sqrt(k))          */
    idx_t threshold = p * p + p;     /* p^2 + p                       */
    idx_t n_row = (k < threshold) ? (2 * p - 1) : (2 * p);  /* row index */

    /* Compute start of the row and offset within that row. */
    idx_t S_prev = (n_row & 1) ? p * p : p * (p + 1);
    idx_t offset = k - S_prev;
    idx_t start = p;                 /* ceil(n_row/2) == p            */
    idx_t m = start + offset;        /* column index in the triangle   */

    /* Return the binomial coefficient C(m+1, n_row-m). */
    return binom_idx(m + 1, n_row - m);
}
