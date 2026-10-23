/*
 * A143773  Number of partitions of n such that every part is divisible by
 *          number of parts.
 * https://oeis.org/A143773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 95, u16 n <= 419, u32 n <= 2159, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A143773: Number of partitions of n such that every part is divisible by
 * the number of parts.
 *
 * Formula: a(n) = Σ_{k|n} p_k(n/k)
 * where p_k(m) is the number of partitions of m into exactly k positive parts.
 * p_k(m) satisfies p_k(m) = 0 for m < k, p_0(0)=1, and
 * p_k(m) = p_k(m-k) + p_{k-1}(m-1).
 *
 * This implementation builds a dynamic table of p_k(m) up to the
 * largest required k (⌊√n⌋) and largest m (n) across all calls.
 * The table is rebuilt only when a larger n or k is requested.
 */

#include <stdlib.h>

/* --- integer square root for uint64_t --------------------------------- */
static inline idx_t isqrt_uint64(idx_t x) {
    idx_t lo = 0, hi = x;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        /* use 128-bit multiplication to avoid overflow */
        if ((unsigned __int128)mid * mid <= x) {
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return hi;
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n) {
    /* static state for memoization */
    static seq_t **dp = NULL;
    static idx_t Kmax = 0;  /* maximum k for which dp exists   */
    static idx_t Nmax = 0;  /* maximum m for which dp exists   */

    /* required limits for this n */
    idx_t needK = isqrt_uint64(n);
    idx_t needN = n;

    /* rebuild table if needed */
    if (needN > Nmax || needK > Kmax) {
        /* allocate new table */
        seq_t **newdp = (seq_t **)calloc((size_t)(needK + 1), sizeof(seq_t *));
        for (idx_t k = 0; k <= needK; ++k) {
            newdp[k] = (seq_t *)calloc((size_t)(needN + 1), sizeof(seq_t));
        }

        /* base case p_0(0) = 1 */
        newdp[0][0] = 1;

        /* dynamic programming: p_k(m) = p_k(m-k) + p_{k-1}(m-1) */
        for (idx_t k = 1; k <= needK; ++k) {
            for (idx_t m = 1; m <= needN; ++m) {
                if (m < k) {
                    newdp[k][(size_t)m] = 0;
                } else {
                    seq_t a1 = newdp[k][(size_t)(m - k)];    /* p_k(m-k) */
                    seq_t a2 = newdp[k - 1][(size_t)(m - 1)];/* p_{k-1}(m-1) */
                    newdp[k][(size_t)m] = a1 + a2;
                }
            }
        }

        /* free old table */
        if (dp) {
            for (idx_t k = 0; k <= Kmax; ++k) {
                free(dp[k]);
            }
            free(dp);
        }

        dp = newdp;
        Kmax = needK;
        Nmax = needN;
    }

    /* sum over divisors k of n (only k ≤ √n are needed) */
    seq_t sum = 0;
    for (idx_t k = 1; k <= Kmax; ++k) {
        if (n % k == 0) {
            sum += dp[k][(size_t)(n / k)];
        }
    }

    return sum;
}
