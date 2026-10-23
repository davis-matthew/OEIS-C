/*
 * A188160  For an unordered partition of n with k parts, remove 1 from each
 *          part and append the number k to get a new partition until a
 *          partition is repeated. a(n) gives the maximum steps to reach a
 *          period considering all unordered partitions of n.
 * https://oeis.org/A188160
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A188160: maximum steps to reach a period
   by iteratively removing 1 from each part of an unordered partition
   and appending the number of parts.
   The sequence can be described by the following piecewise formulas:
   - For n=1: a(1)=0,   n=2: a(2)=1.
   - Triangular numbers T(k)=k(k+1)/2, k>=1:
         a(T(k)) = k^2 - k.
   - For k>=2:
         M_k = (k^2 + 2k - (k mod 2)) / 2
         a(M_k)   = M_k
         a(M_k+1) = M_k+1
   - For k>=4:
         n = T(k)-1 - j,   0 <= j <= (k-4)/2:
                a(n) = k^2 - k - 2 - (k+1) * j
         n = T(k)+1 + j,   0 <= j <= (k-4)/2:
                a(n) = k^2 - k - k * j
   All other n are covered by the above ranges. */
/* Implementation below computes a(n) directly, using O(√n) time. */

#include <stdbool.h>
#include <stdint.h>

static inline idx_t floor_sqrt_u64(idx_t x) {
    /* integer square root using binary search */
    idx_t lo = 0, hi = 1;
    while (hi <= x / hi) {
        hi <<= 1;
    }
    while (lo + 1 < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (mid <= x / mid) {
            lo = mid;
        } else {
            hi = mid;
        }
    }
    return lo;
}

seq_t a(idx_t n) {
    /* special cases */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;

    /* determine an upper bound for k */
    idx_t maxK = floor_sqrt_u64(8 * n + 1) + 2; /* k ≈ sqrt(2n) */

    for (idx_t k = 1; k <= maxK; ++k) {
        /* Triangular number T(k) = k(k+1)/2 */
        seq_t T_k = ((seq_t)k * (k + 1)) / 2;
        if (T_k == n) {
            /* a(T(k)) = k^2 - k */
            return ((seq_t)k * k) - k;
        }

        /* Case 2: M_k and M_k+1, k >= 2 */
        if (k >= 2) {
            seq_t M_k = ((seq_t)k * k + 2 * k - (k & 1)) / 2;
            if (n == M_k || n == M_k + 1) {
                return (seq_t)n;  /* a(n) = n for these n */
            }
        }

        /* Case 3 and Case 4: only for k >= 4 */
        if (k >= 4) {
            /* Case 3: n = T(k)-1 - j, 0 <= j <= (k-4)/2 */
            long long diff3 = (long long)(T_k - 1) - (long long)n;
            if (diff3 >= 0 && diff3 <= ((long long)(k - 4)) / 2) {
                seq_t val = ((seq_t)k * k) - k - 2 - ((seq_t)(k + 1) * diff3);
                return val;
            }

            /* Case 4: n = T(k)+1 + j, 0 <= j <= (k-4)/2 */
            long long diff4 = (long long)n - (long long)(T_k + 1);
            if (diff4 >= 0 && diff4 <= ((long long)(k - 4)) / 2) {
                seq_t val = ((seq_t)k * k) - k - ((seq_t)k * diff4);
                return val;
            }
        }
    }

    /* In theory, all n >= 1 should be handled above. */
    return (seq_t)0;
}
