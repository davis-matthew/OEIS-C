/*
 * A053168  Hamming weights (or nonlinearity) of degree 4 rotation-symmetric
 *          functions.
 * https://oeis.org/A053168
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  hard, more, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 13, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) for sequence A053168:
 * dynamic programming over first 3 bits, last 3 bits and
 * parity of 4‑consecutive-ones windows. */
/* Complexity: O(n).  Works for any n ≥ 4. */

#include <stdint.h>
#include <stdlib.h>

/* Parameters: seq_t – arbitrary unsigned integer type supplied by the build.
 *             idx_t – unsigned 64‑bit index supplied by the build.  */
seq_t a(idx_t n)
{
    /* The defined rotation‑symmetric function for n=4 has weight 1. */
    if (n == 4) return (seq_t)1;

    /* For n ≥ 5 we use the DP described above. */
    /* State encoding: first (3 bits), last (3 bits), parity (1 bit).
     *  index = (first << 4) | (last << 1) | parity      (0 … 127)   */
    const size_t S = 128;
    seq_t dp[S];
    seq_t dp_next[S];

    /* initialise all counts to zero */
    for (size_t i = 0; i < S; ++i) dp[i] = 0;

    /* initial states: first bits equal last bits, parity = 0 */
    for (size_t first = 0; first < 8; ++first) {
        size_t idx = (first << 4) | (first << 1) | 0;
        dp[idx] = 1;
    }

    /* process bits 4 … n (n‑3 steps) */
    for (idx_t pos = 4; pos <= n; ++pos) {
        for (size_t i = 0; i < S; ++i) dp_next[i] = 0;
        for (size_t state = 0; state < S; ++state) {
            seq_t cnt = dp[state];
            if (!cnt) continue;
            size_t first  = state >> 4;
            size_t last   = (state >> 1) & 7;   /* bits: l0,l1,l2 */
            size_t parity = state & 1;

            /* add next bit b (0 or 1) */
            for (int b = 0; b <= 1; ++b) {
                size_t m = ((last == 7) && (b == 1)) ? 1 : 0;
                size_t new_parity = parity ^ m;
                /* new last: (last >> 1) keeps l1,l2, b becomes new l2 */
                size_t new_last = (((last >> 1) | ((size_t)b << 2)) & 7);
                size_t new_idx = (first << 4) | (new_last << 1) | new_parity;
                dp_next[new_idx] += cnt;
            }
        }
        /* swap dp and dp_next */
        for (size_t i = 0; i < S; ++i) dp[i] = dp_next[i];
    }

    /* compute final answer adding wrap‑around windows */
    seq_t ans = 0;
    for (size_t state = 0; state < S; ++state) {
        seq_t cnt = dp[state];
        if (!cnt) continue;
        size_t first = state >> 4;
        size_t last  = (state >> 1) & 7;
        size_t parity = state & 1;

        unsigned int f0 = first & 1;
        unsigned int f1 = (first >> 1) & 1;
        unsigned int f2 = (first >> 2) & 1;
        unsigned int l0 = last & 1;
        unsigned int l1 = (last >> 1) & 1;
        unsigned int l2 = (last >> 2) & 1;

        unsigned int w1 = l0 & l1 & l2 & f0;
        unsigned int w2 = l1 & l2 & f0 & f1;
        unsigned int w3 = l2 & f0 & f1 & f2;
        unsigned int wrap_parity = (w1 ^ w2 ^ w3);

        if ((parity ^ wrap_parity) & 1) {
            ans += cnt;
        }
    }
    return ans;
}
