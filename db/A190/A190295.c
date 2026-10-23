/*
 * A190295  A055134(n,k)*k.
 * https://oeis.org/A190295
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) enumerates the triangle T(n,k)=k*C(n,k)*(n-1)^(n-k) read by
 *            rows (OEIS A190295). The index n (starting at 1) is mapped to a
 *            specific row r and column k by r(r+1)/2 >= n and (r-1)r/2 < n ,
 *            then k = n - (r-1)r/2. The term is a(n) = T(r,k)
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   big_pow, binom, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/big_pow.h"
#include "../utils/seq_mul.h"

static inline idx_t binary_search_row(idx_t n) {
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        idx_t tri = mid * (mid + 1) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n) {
    /* Determine row r and column k for the n-th term */
    idx_t r = binary_search_row(n);
    idx_t prev = r * (r - 1) / 2;
    idx_t k = n - prev;                     /* 1 ≤ k ≤ r */

    /* Compute T(r,k) = k * C(r,k) * (r-1)^(r-k) */
    seq_t res = binom(r, k);                /* C(r,k)   */
    if (r > 1) {                            /* (r-1)^(r-k) */
        seq_t pow = big_pow(r - 1, r - k);
        res = seq_mul(res, pow);
    }
    /* multiply by k */
    res = seq_mul(res, k);
    return res;
}
