/*
 * A161065  Number of partitions of n into powers of two minus one where every
 *          part appears at least 3 times.
 * https://oeis.org/A161065
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into parts 2^k-1, each part
 *            appearing at least three times
 * Range:     u8 n <= 101, u16 n <= 407, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 96 terms through n = 974 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;

    /* Dynamic programming arrays */
    seq_t *dp   = mem_new(seq_t, n + 1);
    seq_t *newdp = mem_new(seq_t, n + 1);
    if (!dp || !newdp) {
        /* allocation failure: return 0 */
        if (dp) mem_free(dp);
        if (newdp) mem_free(newdp);
        return 0;
    }
    dp[0] = 1;

    /* List of part sizes 2^k-1 up to n */
    idx_t parts[64];
    size_t pcnt = 0;
    for (idx_t val = 1; val <= n; val = (val << 1) + 1) {
        parts[pcnt++] = val;
    }

    for (size_t pi = 0; pi < pcnt; ++pi) {
        idx_t s = parts[pi];

        /* copy old dp into newdp */
        for (idx_t i = 0; i <= n; ++i) newdp[i] = dp[i];

        /* process each residue class modulo s */
        for (idx_t r = 0; r < s; ++r) {
            if (r > n) break;          /* no indices in this class */
            idx_t m = (n - r) / s + 1; /* number of elements in the class */

            /* prefix sums of dp over this class */
            seq_t *pref = mem_new(seq_t, m + 1);
            if (!pref) { /* fallback on allocation failure */
                mem_free(dp);
                mem_free(newdp);
                return 0;
            }
            pref[0] = 0;
            for (idx_t k = 0; k < m; ++k) {
                pref[k + 1] = pref[k] + dp[r + k * s];
            }

            /* add contributions of using the part at least 3 times */
            for (idx_t k = 0; k < m; ++k) {
                if (k >= 3) {
                    newdp[r + k * s] += pref[k - 2];
                }
            }
            mem_free(pref);
        }

        /* swap dp and newdp for next part */
        seq_t *tmp = dp;
        dp = newdp;
        newdp = tmp;
    }

    seq_t result = dp[n];
    mem_free(dp);
    mem_free(newdp);
    return result;
}
