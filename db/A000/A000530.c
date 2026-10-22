/*
 * A000530  Let p(n, s, x) be predicate that number of occurrences of s's in x
 *          >= 2*n - the length of the longest sequence of s's in x. Then
 *          a(n)=#{x in {0,1}* | x ends in 0 and p(n,0,x) and (there is no
 *          prefix y of x such that p(n,0,y) or p(n,1,y))}.
 * https://oeis.org/A000530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard, more
 * Formula:   no closed form; dynamic programming over all binary strings
 *            (counted, not enumerated). The state is (#0s, #1s, longest
 *            0-run, longest 1-run, last bit, current run length). No proper
 *            prefix may satisfy #s + longest_run_s >= 2n. The function counts
 *            the strings that reach #0 + longest_0_run >= 2n at...
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n_idx) {
    long long n = (long long)n_idx;
    long long C = 2 * n;           /* c0, c1 in [0, 2n) */
    long long M = n;               /* m0, m1, r in [0, n) */
    size_t total = (size_t)(C * C * M * M * 2 * M);
    uint64_t *dp = (uint64_t *)calloc(total, sizeof(uint64_t));
    uint64_t ans = 0;
    long long c0, c1, m0, m1, b, r, L;
#define IDX(c0_, c1_, m0_, m1_, b_, r_) \
    ((((((size_t)(c0_) * C + (c1_)) * M + (m0_)) * M + (m1_)) * 2 + (b_)) * M + (r_))

    if (!dp) return (seq_t)0;
    dp[IDX(0, 0, 0, 0, 0, 0)] = 1;   /* empty string */

    for (L = 0; L <= 2 * C - 2; L++) {
        for (c0 = 0; c0 < C && c0 <= L; c0++) {
            c1 = L - c0;
            if (c1 >= C) continue;
            for (m0 = 0; m0 < M; m0++)
            for (m1 = 0; m1 < M; m1++)
            for (b = 0; b < 2; b++)
            for (r = 0; r < M; r++) {
                uint64_t v = dp[IDX(c0, c1, m0, m1, b, r)];
                long long nr, nm, nc;
                if (!v) continue;
                /* append 0 */
                nr = (b == 0) ? r + 1 : 1;
                nm = nr > m0 ? nr : m0;
                nc = c0 + 1;
                if (nc + nm >= 2 * n) {
                    ans += v;
                } else {
                    dp[IDX(nc, c1, nm, m1, 0, nr)] += v;
                }
                /* append 1 */
                nr = (b == 1) ? r + 1 : 1;
                nm = nr > m1 ? nr : m1;
                nc = c1 + 1;
                if (nc + nm < 2 * n) {
                    dp[IDX(c0, nc, m0, nm, 1, nr)] += v;
                }
            }
        }
    }
#undef IDX
    free(dp);
    return (seq_t)ans;
}
