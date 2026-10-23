/*
 * A168644  Triangle read by rows: T(n, k) = [x^k] p(x,n), where p(x,0) = 1,
 *          p(x,n) = (7 - n)*(1+x)^n - (6-n)*(1 + x^n) for 1 <= n <= 5, and
 *          p(x,n) = 5*(1 + x)^n - Sum_{i=0..3} (Sum_{j=0..i} binomial(n,
 *          j)*(x^j + x^(n-j))) + (1/6)*n*(n - 1)*(n - 5)*x^(n-3) for n >= 6.
 * https://oeis.org/A168644
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl, less
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 158, u32 n <= 574, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A168644
 * Triangle read by rows:  T(n,k) = [x^k] p(x,n)
 *
 * p(x,0) = 1
 * p(x,n) = (7-n)*(1+x)^n - (6-n)*(1 + x^n)          for 1 ≤ n ≤ 5
 * p(x,n) = 5*(1+x)^n
 *        - Σ_{j=0..3} (4-j)·C(n,j)·(x^j + x^(n-j))
 *        + [n(n‑1)(n‑5)/6]·x^(n‑3)                    for n ≥ 6
 *
 * a(m) is the m‑th entry of the triangle read left‑to‑right.
 *
 * This implementation evaluates the coefficient T(r,k) in linear time
 * (O(r) ≤ O(√n)).  It uses the supplied binom(idx_t a, idx_t b) helper
 * to compute binomial coefficients exactly in the sequence type.
 */

#include "../utils/binom.h"

seq_t a(idx_t n) {
    /* Find row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2. */
    idx_t r = 0;
    while (1) {
        __uint128_t tri_next = (__uint128_t)(r + 1) * (r + 2) / 2;
        if (tri_next > n) break;
        r++;
    }
    __uint128_t row_start = (__uint128_t)r * (r + 1) / 2;
    idx_t k = (idx_t)(n - row_start);          /* column index */

    if (r == 0)                /* first row has only one entry */
        return (seq_t)1;

    if (r <= 5) {
        seq_t coeff = (seq_t)(7 - (int)r) * binom(r, k);
        seq_t sub   = 0;
        if (k == 0 || k == r)
            sub = (seq_t)(6 - (int)r);
        return coeff - sub;
    } else {
        /* r >= 6 */
        seq_t result = (seq_t)5 * binom(r, k);

        /* subtract the terms from j=0..3 */
        for (int j = 0; j <= 3; ++j) {
            idx_t j_idx = (idx_t)j;
            seq_t c     = binom(r, j_idx);
            if (k == j_idx)
                result -= (seq_t)(4 - j) * c;
            if (k == r - j_idx)
                result -= (seq_t)(4 - j) * c;
        }

        /* add the extra term if k == r-3 */
        if (k == r - 3) {
            seq_t extra =
                (seq_t)r * (seq_t)(r - 1) * (seq_t)(r - 5);
            extra /= 6;           /* integer division */
            result += extra;
        }

        return result;
    }
}
