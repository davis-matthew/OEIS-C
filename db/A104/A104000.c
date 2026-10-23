/*
 * A104000  Square array T(r,m) read by antidiagonals: number of cyclically
 *          reduced words of length m in F_r.
 * https://oeis.org/A104000
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (2r-1)^m + 1 + (r-1)(1+(-1)^m), where r and m are
 *            obtained from n by reading the square array T(r,m) = (2r-1)^m +
 *            1 + (r-1)(1+(-1)^m) along antidiagonals. For a given n (≥1) find
 *            t such that t(t+1)/2 ≥ n; let j = n - (t-1)t/2 (1 ≤ j ≤ t), then
 *            r = t - j + 1, m = j
 * Range:     u8 n <= 17, u16 n <= 41, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* find minimal t with t*(t+1)/2 >= n */
    idx_t t = 0;
    while ((t * (t + 1ULL)) / 2ULL < n) {
        ++t;
    }
    /* offset within this antidiagonal */
    idx_t prev = ((t - 1ULL) * t) / 2ULL;          /* terms before sum t+1 */
    idx_t j = n - prev;                           /* 1-indexed within sum */
    idx_t r = t - j + 1ULL;                       /* row index */
    idx_t m = j;                                  /* column index */

    /* compute (2r-1)^m */
    seq_t base = (seq_t)(2ULL * r - 1ULL);
    seq_t pow_val = 1;
    for (idx_t k = 0; k < m; ++k) {
        pow_val *= base;
    }

    /* assemble the formula */
    seq_t result = pow_val + 1;
    if ((m & 1ULL) == 0ULL) { /* m even */
        result += (seq_t)(2ULL * (r - 1ULL));
    }
    return result;
}
