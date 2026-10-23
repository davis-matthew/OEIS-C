/*
 * A184708  Number of strings of numbers x(i=1..8) in 0..n with sum i*x(i)
 *          equal to n*8.
 * https://oeis.org/A184708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of non‑negative integer solutions * (x1,…,x8) with
 *            each 0 ≤ xi ≤ n and ∑ i·xi = 8·n. * Computation is via dynamic
 *            programming over the 8 weights, * using a sliding‑window
 *            convolution for each weight
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 48, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

#ifndef __cplusplus
/* C: use calloc / free and cast explicitly */
static inline seq_t *allocate_dp(size_t sz) { return (seq_t*)calloc(sz, sizeof(seq_t)); }
static inline void free_dp(seq_t *p) { free(p); }
#else
/* C++: use new[]/delete[] for value-initialisation */
static inline seq_t *allocate_dp(size_t sz) { return new seq_t[sz](); }
static inline void free_dp(seq_t *p) { delete[] p; }
#endif

seq_t a(idx_t n)
{
    /* target weighted sum */
    const size_t target = (size_t)8u * (size_t)n;

    /* Allocate DP arrays */
    seq_t *dp    = allocate_dp(target + 1);
    seq_t *newdp = allocate_dp(target + 1);
    if (!dp || !newdp) { free_dp(dp); free_dp(newdp); return 0; }

    dp[0] = 1;   /* one way to reach sum 0 */

    /* Process each weight i = 1…8 */
    for (int i = 1; i <= 8; ++i) {
        /* For each residue class modulo i */
        for (int r = 0; r < i; ++r) {
            seq_t sum = 0;
            const size_t bound = ((size_t)(n + 1)) * (size_t)i;  /* window width in index units */

            for (size_t s = r; s <= target; s += (size_t)i) {
                sum += dp[s];
                if (s >= bound) sum -= dp[s - bound];
                newdp[s] = sum;
            }
        }
        /* Swap pointers for the next weight */
        seq_t *tmp = dp;
        dp = newdp;
        newdp = tmp;
    }

    seq_t result = dp[target];

    free_dp(dp);
    free_dp(newdp);
    return result;
}
