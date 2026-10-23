/*
 * A161101  Number of partitions of n into squares where every part appears at
 *          least 13 times.
 * https://oeis.org/A161101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into square parts where every
 *            used square appears at least 13 times. G.f.: ∏_{k≥1} (1 +
 *            x^{13k²}/(1 - x^{k²}))
 * Range:     u8 n <= 284, u16 n <= 864, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle n = 0 gracefully; offset starts at 1 in OEIS. */
    if (n == 0) return (seq_t)0;

    /* Convert n to platform size_t for array indexing. */
    size_t N = (size_t)n;

    /* Allocate DP arrays.  Use explicit casts to satisfy C++. */
    seq_t *dp      = (seq_t*)calloc(N + 1, sizeof(seq_t));
    seq_t *dp_next = (seq_t*)malloc  ((N + 1) * sizeof(seq_t));

    if (!dp || !dp_next) {
        /* Allocation failed – return 0 (unlikely in tests). */
        free(dp); free(dp_next);
        return (seq_t)0;
    }

    dp[0] = (seq_t)1;   /* empty partition */

    /* Iterate over square values s = k² up to n. */
    for (idx_t k = 1; ; ++k) {
        idx_t s = k * k;
        if (s > n) break;

        /* Copy current dp into dp_next (skipping this part). */
        for (size_t i = 0; i <= N; ++i) dp_next[i] = dp[i];

        /* Sum array for residues modulo s.  Allocate once per s. */
        size_t s_sz = (size_t)s;
        seq_t *sum_by_rem = (seq_t*)calloc(s_sz, sizeof(seq_t));
        if (!sum_by_rem) {
            free(dp); free(dp_next);
            return (seq_t)0;
        }

        /* Minimum amount contributed by this part: 13 copies. */
        idx_t min = 13 * s;
        /* Process sums that can include this part at least 13 times. */
        for (idx_t i = min; i <= n; ++i) {
            size_t r = (size_t)(i % s);
            idx_t idx = i - min;              /* index of dp to add in sum */
            size_t r_idx = (size_t)(idx % s); /* same as r */

            /* Extend running sum for this residue. */
            sum_by_rem[r_idx] += dp[idx];

            /* Add accumulated ways to dp_next. */
            dp_next[i] += sum_by_rem[r];
        }

        free(sum_by_rem);

        /* Prepare for next iteration. Swap dp and dp_next pointers. */
        seq_t *tmp = dp;
        dp = dp_next;
        dp_next = tmp;
    }

    seq_t result = dp[N];    /* a(n) */

    /* Free allocated memory. */
    free(dp);
    free(dp_next);

    return result;
}
