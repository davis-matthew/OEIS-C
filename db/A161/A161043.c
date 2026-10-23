/*
 * A161043  Number of partitions of n into odd numbers where every part
 *          appears at least 7 times.
 * https://oeis.org/A161043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 112, u16 n <= 275, u32 n <= 785, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of partitions of n into odd parts where each part appears at least 7 times.
   Computed by dynamic programming over odd part sizes. */
#include <stdlib.h>
#include <string.h>
#ifdef __cplusplus
#include <vector>
#include <algorithm>
#endif

seq_t a(idx_t n) {
    if (n < 7) return 0; /* below threshold no partition satisfies the condition */

    idx_t N = n;          /* make a local copy to use for array sizes */

#ifdef __cplusplus
    std::vector<seq_t> dp_prev(N + 1, 0), dp_new(N + 1, 0);
#else
    seq_t *dp_prev = calloc((size_t)(N + 1), sizeof(seq_t));
    seq_t *dp_new  = calloc((size_t)(N + 1), sizeof(seq_t));
    if (!dp_prev || !dp_new) return 0; /* allocation failed – unlikely in tests */
#endif

    dp_prev[0] = 1;  /* base case: one way to make sum 0 (empty partition) */

    for (idx_t k = 1; k <= N; k += 2) {          /* odd part sizes only */
        /* copy previous dp to new dp */
#ifdef __cplusplus
        for (idx_t t = 0; t <= N; ++t) dp_new[t] = dp_prev[t];
#else
        memcpy(dp_new, dp_prev, (size_t)(N + 1) * sizeof(seq_t));
#endif

        /* unlimited copies of part k (complete knapsack) */
        for (idx_t t = k; t <= N; ++t) {
#ifdef __cplusplus
            dp_new[t] += dp_new[t - k];
#else
            dp_new[t] = dp_new[t] + dp_new[t - k];
#endif
        }

        /* subtract contributions with 1 to 6 copies of part k */
        for (idx_t t = 0; t <= N; ++t) {
#ifdef __cplusplus
            seq_t sub = 0;
            for (idx_t r = 1; r <= 6 && t >= r * k; ++r) sub += dp_prev[t - r * k];
            if (dp_new[t] >= sub) dp_new[t] -= sub;
            else dp_new[t] = 0;      /* safety: should not happen */
#else
            seq_t sub = 0;
            for (idx_t r = 1; r <= 6 && t >= r * k; ++r)
                sub = sub + dp_prev[t - r * k];
            if (dp_new[t] >= sub)
                dp_new[t] = dp_new[t] - sub;
            else
                dp_new[t] = 0;
#endif
        }

        /* swap dp_prev and dp_new for next iteration */
#ifdef __cplusplus
        dp_prev.swap(dp_new);
#else
        seq_t *tmp = dp_prev;
        dp_prev = dp_new;
        dp_new = tmp;
#endif
    }

    seq_t result = dp_prev[N];

#ifndef __cplusplus
    free(dp_prev);
    free(dp_new);
#endif
    return result;
}
