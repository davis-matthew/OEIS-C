/*
 * A097797  Number of partitions of n into deficient numbers.
 * https://oeis.org/A097797
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into deficient numbers
 * Range:     u8 n <= 17, u16 n <= 47, u32 n <= 145, u64 n <= 494,
 *            u128 n <= 1796, big any n
 * Helpers:   build_spf, sigma
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_spf.h"
#include "../utils/sigma.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, but keep safe */

    /* 1. Build spf up to n. */
    idx_t *spf = NULL;
    build_spf(n, &spf);
    if (!spf) return 0;            /* allocation failure */

    /* 2. Gather all deficient numbers ≤ n. */
    size_t cap = 64;
    size_t cnt = 0;
    idx_t *def = (idx_t *)malloc(cap * sizeof(idx_t));
    if (!def) { free(spf); return 0; }

    for (idx_t i = 1; i <= n; ++i) {
        seq_t sgi = sigma(i, spf);
        if (sgi < (seq_t)2 * (seq_t)i) {    /* deficient */
            if (cnt == cap) {
                cap *= 2;
                idx_t *tmp = (idx_t *)realloc(def, cap * sizeof(idx_t));
                if (!tmp) { free(def); free(spf); return 0; }
                def = tmp;
            }
            def[cnt++] = i;
        }
    }

    /* 3. Dynamic programming over deficient numbers. */
    seq_t *dp = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    if (!dp) { free(def); free(spf); return 0; }
    dp[0] = 1;

    for (size_t k = 0; k < cnt; ++k) {
        idx_t d = def[k];
        for (idx_t s = d; s <= n; ++s) {
            dp[s] += dp[s - d];
        }
    }

    seq_t ans = dp[n];

    /* 4. Clean up. */
    free(def);
    free(spf);
    free(dp);

    return ans;
}
