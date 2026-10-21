/*
 * A055134  Triangle read by rows: T(n,k) = number of labeled endofunctions on
 *          n points with k fixed points.
 * https://oeis.org/A055134
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = C(r,c) * (r-1)^(r-c), where r = floor((sqrt(8n+1)-1)/2)
 *            (row index) c = n - r*(r+1)/2 (column index)
 * Range:     u8 n <= 14, u16 n <= 27, u32 n <= 65, u64 n <= 152,
 *            u128 n <= 405, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for OEIS sequence A055134.  */
seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
    idx_t lo = 0;
    idx_t hi = n;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;
        idx_t start = (mid * (mid + 1)) / 2;
        if (start <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    idx_t r = lo;

    /* Handle the trivial 0‑point case. */
    if (r == 0)
        return (seq_t)1;

    /* Column c and start index of row r. */
    idx_t start = (r * (r + 1)) / 2;
    idx_t c     = n - start;

    /* Compute binomial coefficient C(r,c) multiplicatively. */
    idx_t k = c;
    if (k > r - k)
        k = r - k;
    seq_t binom = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        binom = binom * (seq_t)(r - k + i);
        binom = binom / (seq_t)i;  /* exact division */
    }

    /* Compute (r-1)^(r-c) by repeated multiplication. */
    seq_t base = (seq_t)(r - 1);
    idx_t exp  = r - c;
    seq_t pow  = (seq_t)1;
    for (idx_t e = 0; e < exp; ++e)
        pow = pow * base;

    return binom * pow;
}
