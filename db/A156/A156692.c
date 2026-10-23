/*
 * A156692  Triangle T(n, k, m) = t(n,m)/( t(k,m) * t(n-k,m) ) with T(n, 0, m)
 *          = T(n, n, m) = 1, where t(n, m) = Product_{j=1..n}
 *          Product_{i=1..j-1} ( 1 - (m+1)*(i+1) ) and m = 3, read by rows.
 * https://oeis.org/A156692
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = T(r,k,3) with r = floor((sqrt(8n+1)-1)/2), k = n -
 *            r(r+1)/2. T(n,k,3) = ∏_{i=1}^{n-1} (-(4i+3))^{e_i}, where e_i =
 *            i if i ≤ k-1 and i ≤ n-k-1 n-k if i ≤ k-1 and i > n-k-1 k if i >
 *            k-1 and i ≤ n-k-1 n-i otherwise
 * Range:     i8 n <= 7, i16 n <= 16, i32 n <= 23, i64 n <= 39, i128 n <= 70,
 *            big any n
 * Helpers:   big_pow, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/big_pow.h"
#include "../utils/floor_sqrt.h"

static seq_t compute_T(idx_t n, idx_t k)
{
    if (k == 0 || k == n)
        return (seq_t)1;          /* edge entries are 1 */

    seq_t res = (seq_t)1;

    for (idx_t i = 1; i < n; ++i) {
        seq_t base = -(seq_t)(4 * i + 3);
        idx_t e;

        if (i <= k - 1 && i <= (n - k) - 1) {
            e = i;
        } else if (i <= k - 1) {
            e = n - k;
        } else if (i <= (n - k) - 1) {
            e = k;
        } else {
            e = n - i;
        }

        if (e == 0) continue;

        seq_t pow_val = big_pow(base, (seq_t)e);
        res *= pow_val;
    }

    return res;
}

seq_t a(idx_t n)
{
    /* Locate the row r and column k for the linear index n. */
    seq_t tmp   = (seq_t)8 * n + 1;
    seq_t sq    = floor_sqrt(tmp);
    idx_t r     = (idx_t)((sq - 1) / 2);
    idx_t offset = r * (r + 1) / 2;
    idx_t k     = n - offset;

    return compute_T(r, k);
}
