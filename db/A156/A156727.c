/*
 * A156727  Triangle T(n, k, m) = t(n,m)/( t(k,m) * t(n-k,m) ) with T(n, 0, m)
 *          = T(n, n, m) = 1, where t(n, m) = Product_{j=1..n}
 *          Product_{i=1..j-1} ( 1 - (m+1)*(3*i-2) ) and m = 3, read by rows.
 * https://oeis.org/A156727
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = Triangle T(n,k,3) read row by row, where * T(n,k,3) =
 *            t(n)/(t(k)*t(n-k)) and * t(m) = ∏_{i=1}^{m-1} (9-12*i)^{(m-i)}.
 *            * The n-th term corresponds to row r and position k with *
 *            r*(r+1)/2 <= n < (r+1)*(r+2)/2, k = n - r*(r+1)/2
 * Range:     i8 n <= 10, i16 n <= 15, i32 n <= 22, i64 n <= 38, i128 n <= 58,
 *            big any n
 * Helpers:   big_pow, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_mul.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    /* Find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t low = 0, high = n, r = 0, best = 0;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 sum = (unsigned __int128)mid * (mid + 1) / 2;
        if (sum <= n) {
            best = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    r = best;
    /* Position k within the row */
    idx_t k = n - (idx_t)((unsigned __int128)r * (r + 1) / 2);
    idx_t r_minus_k = r - k;
    /* Compute T(r,k,3) */
    seq_t result = (seq_t)1;
    for (idx_t i = 1; i < r; ++i) {
        int e = (int)(r - i);
        if (i < k) e -= (int)(k - i);
        if (i < r_minus_k) e -= (int)(r_minus_k - i);
        if (e <= 0) continue;
        seq_t base = (seq_t)(9 - (long long)12 * (long long)i);
        seq_t pow = big_pow(base, (seq_t)e);
        result = seq_mul(result, pow);
    }
    return result;
}
