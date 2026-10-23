/*
 * A161071  Number of partitions of n into powers of two minus one where every
 *          part appears at least 9 times.
 * https://oeis.org/A161071
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 179, u16 n <= 649, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = number of partitions of n into parts of the form 2^k−1,
 *              where each part appears at least 9 times.  Offset 1.
 *
 * The implementation uses generating‑function convolution.
 * For each part size s = 2^k−1 the generating function is
 *      1 + x^{9s} + x^{10s} + ⋯
 * which in coefficient form means for every n:
 *      newdp[n] = dp[n] + dp[n−9s] + dp[n−10s] + …
 * We compute this efficiently by processing each residue class modulo s
 * separately and using prefix sums.  The algorithm runs in O(n·m)
 * where m is the number of distinct parts (≈ log₂(n)).
 *
 * All arithmetic uses the types seq_t and idx_t as required by the
 * build system.  No overflow occurs with normal 64‑bit integers because
 * all intermediate values are stored in seq_t, which may be arbitrary
 * precision when needed.
 */
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;
    /* Allocate two DP arrays of length n+1 */
    size_t size = (size_t)n + 1;
    seq_t *dp      = (seq_t *)calloc(size, sizeof(seq_t));
    seq_t *newdp   = (seq_t *)calloc(size, sizeof(seq_t));
    if (!dp || !newdp) return 0;          /* Out‑of‑memory guard */
    dp[0] = 1;                           /* Empty partition */

    /* Generate all part sizes <= n : 2^k − 1 */
    idx_t s;
    for (idx_t k = 1; ; ++k) {
        s = (idx_t)((1ULL << k) - 1ULL);
        if (s > n) break;
        /* Convolution with 1 + x^{9s} + x^{10s} + …  */
        /* For each residue modulo s */
        for (idx_t r = 0; r < s; ++r) {
            size_t len = (n >= r) ? ((n - r) / s + 1) : 0;
            seq_t prefix = 0;
            for (size_t m = 0; m < len; ++m) {
                idx_t t = r + m * s;          /* Current sum */
                if (m >= 9) {
                    /* Add dp[t−9s] to the running prefix sum */
                    size_t idx = m - 9;
                    seq_t *arr = dp + (r + idx * s); /* dp[ t-9s ] */
                    prefix += *arr;
                }
                newdp[t] = dp[t] + prefix;   /* dp[t] + sum_{k≥9} dp[t−k*s] */
            }
        }
        /* Prepare for next part: swap dp and newdp */
        seq_t *tmp = dp;
        dp = newdp;
        newdp = tmp;
    }

    seq_t result = dp[n];
    free(dp);
    free(newdp);
    return result;
}
