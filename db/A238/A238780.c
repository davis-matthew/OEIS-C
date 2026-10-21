/*
 * A238780  Number of palindromic partitions of n whose greatest part has
 *          multiplicity 4.
 * https://oeis.org/A238780
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of palindromic partitions of n whose greatest part
 *            has multiplicity 4
 * Range:     u8 n <= 43, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
/* seq_t and idx_t are provided by the build system */

#pragma warning(disable:4996) /* Suppress MSVC inlining warnings if any */

seq_t a(idx_t n) {
    idx_t max_p = n / 4;          /* largest part can be at most n/4 */
    seq_t result = 0;
    if (max_p == 0) return result; /* no valid partitions */

    /* maximum half‑sum we will ever need */
    idx_t halfMax = n / 2;
    seq_t *dp = (seq_t *)calloc((size_t)halfMax + 1, sizeof(seq_t));
    if (!dp) return result;          /* out of memory guard */
    dp[0] = (seq_t)1;                /* empty partition */

    idx_t current_B = 0;              /* largest part processed so far */

    for (idx_t p = 1; p <= max_p; ++p) {
        idx_t target_B = p - 1;       /* parts allowed for the rest */

        /* grow the DP table to include part number target_B */
        while (current_B < target_B) {
            ++current_B;
            for (idx_t s = current_B; s <= halfMax; ++s) {
                dp[s] = dp[s] + dp[s - current_B];
            }
        }

        idx_t m = n - (idx_t)4 * p;   /* remaining sum after the 4 copies of part p */

        /* even multiplicity case – all other parts in pairs */
        if ((m & 1) == 0) {
            idx_t half = m >> 1;
            result += dp[half];
        }

        /* odd multiplicity case – one part appears an odd number of times */
        if (target_B >= 1) {
            for (idx_t j = 1; j <= target_B; ++j) {
                if (m < j) continue;
                idx_t diff = m - j;
                if ((diff & 1) == 0) {
                    idx_t half2 = diff >> 1;
                    result += dp[half2];
                }
            }
        }
    }

    free(dp);
    return result;
}
