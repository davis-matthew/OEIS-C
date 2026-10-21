/*
 * A211957  Triangle of coefficients of a polynomial sequence related to the
 *          Morgan-Voyce polynomials A085478.
 * https://oeis.org/A211957
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = T(r,k) Where r,k are obtained from the linear index n by
 *            unfolding the lower‑triangular triangle T(n,k) defined by:
 *            T(n,0) = 1 T(n,k) = n/k * 2^(k-1) * binomial(n + k - 1, 2k - 1)
 *            for k > 0
 * Range:     u8 n <= 17, u16 n <= 49, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Utility for integer square root */
static idx_t isqrt_uint64(idx_t v) {
    idx_t low = 0, high = v;
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        if (mid > 0 && mid > v / mid) {
            high = mid - 1;
        } else {
            low = mid;
        }
    }
    return low;
}

#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    /* Determine row r and column k from flattened index n */
    /* Solve for smallest s such that s(s+1)/2 > n, with s = r+1 */
    idx_t val = (n << 3) + 1;               /* 8*n + 1 */
    idx_t sqrt_val = isqrt_uint64(val);     /* floor(sqrt(8*n+1)) */
    idx_t s = (sqrt_val - 1) / 2 + 1;        /* s = r+1 */
    idx_t r = s - 1;                        /* row index */

    /* Cumulated elements up to previous row */
    idx_t cum_prev = r * (r + 1) / 2;        /* r*(r+1)/2 = previous cumulative count */
    idx_t k = n - cum_prev;                 /* column index (0 <= k <= r) */

    /* Base case: first element of each row */
    if (k == 0) {
        return (seq_t)1;
    }

    /* Compute T(r,k) using the explicit formula:
         T(n,k) = (n/k) * 2^(k-1) * binomial(n + k - 1, 2k - 1)  */
    seq_t bin = binomial(r + k - 1, 2 * k - 1);
    seq_t pow2k = pow2(k - 1);               /* 2^(k-1) */
    seq_t prod = bin * pow2k;                /* bin*2^(k-1) */
    seq_t num = prod * (seq_t)r;             /* (n)*bin*2^(k-1) */
    seq_t result = num / (seq_t)k;           /* divide by k (exact division) */

    return result;
}
