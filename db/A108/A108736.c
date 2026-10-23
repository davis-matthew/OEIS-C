/*
 * A108736  Start with S = {}. For m = 1, 2, 3, ... in turn, examine all 2^m
 *          m-bit strings u in arithmetic order. If u is not a substring of S,
 *          append the minimal number of 0's and 1's to S to remedy this.
 *          Sequence gives S.
 * https://oeis.org/A108736
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: Construct the minimal binary universal sequence by
   iteratively appending missing m‑bit patterns with maximal overlap.
   The sequence a(n) is the (n‑1)th bit of the resulting infinite string.
*/

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* n is 1‑based */
    if (n == 0) return (seq_t)0;   /* undefined, but return 0 */

    /* dynamic circular buffer of bits (values 0 or 1) */
    size_t cap = 64, len = 0;
    uint8_t *S = (uint8_t*)malloc(cap);
    if (!S) return (seq_t)0;       /* allocation failed */

    for (unsigned m = 1; ; ++m) {
        uintptr_t mask = ((uintptr_t)1 << m) - 1;
        size_t patterns = ((size_t)1 << m);
        uint8_t *seen = (uint8_t*)calloc(patterns, 1);
        if (!seen) { free(S); return (seq_t)0; }

        /* scan current S to mark seen patterns of length m */
        uintptr_t window = 0;
        size_t winlen = 0;
        for (size_t i = 0; i < len; ++i) {
            window = ((window << 1) | S[i]) & mask;
            if (winlen < m) ++winlen;
            if (winlen == m) seen[window] = 1;
        }
        /* window now holds last m bits (or less if len < m) */
        /* keep winlen updated for incremental appending */

        /* iterate all m‑bit strings in arithmetic order */
        for (size_t u = 0; u < patterns; ++u) {
            if (seen[u]) continue;   /* already present */

            /* find maximal overlap k (0 <= k <= m) */
            size_t k;
            bool overlapped;
            for (k = m; k > 0; --k) {
                if (k > len) continue;
                overlapped = true;
                for (size_t j = 0; j < k; ++j) {
                    uint8_t bit_s = S[len - k + j];
                    uint8_t bit_u = (u >> (m - 1 - j)) & 1;
                    if (bit_s != bit_u) { overlapped = false; break; }
                }
                if (overlapped) break;   /* largest k found */
            }
            if (k > m) k = 0;   /* safety */

            /* append the remaining (m - k) bits of u */
            for (size_t pos = k; pos < m; ++pos) {
                uint8_t bit = (u >> (m - 1 - pos)) & 1;
                /* push bit */
                if (len == cap) {
                    cap <<= 1;
                    S = (uint8_t*)realloc(S, cap);
                }
                S[len++] = bit;

                /* update rolling window for current m */
                window = ((window << 1) | bit) & mask;
                if (winlen < m) ++winlen;
                if (winlen == m) seen[window] = 1;

                /* if we have reached the desired index return */
                if (len >= n) {
                    free(S);
                    free(seen);
                    return (seq_t)bit;
                }
            }
            /* because we appended, some new patterns may become seen.
               They will be taken into account by marking window above. */
        }
        free(seen);
        /* after finishing all patterns of length m, check if we already
           have enough bits */
        if (len >= n) {
            /* retrieve bit at position n-1 */
            uint8_t bit = S[n - 1];
            free(S);
            return (seq_t)bit;
        }
        /* else continue with next m */
    }
}
