/*
 * A048931  Numbers that are the sum of 6 positive cubes in exactly 3 ways.
 * https://oeis.org/A048931
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 1141, u32 n <= 1141, u64 n <= 1141,
 *            u128 n <= 1141, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A048931: numbers that are the sum of 6 positive cubes in exactly
   three distinct ways.  The implementation pre‑computes all sums of 6
   positive cubes ≤ 26132 and counts the representations.  We keep a
   static cache so that repeated calls are answered in O(1).  The
   enumeration uses a depth‑first search over non‑decreasing tuples,
   which is fast enough for the stated limits. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Upper bound on the searched sums (the largest known term). */
static const idx_t MAX_LIMIT = 26132;

/* Cache state. */
static bool            initialized = false;
static seq_t *         terms       = NULL;   /* array of terms */
static size_t          term_count  = 0;      /* number of terms */

/* Depth‑first search that counts representations of a sum by 6 cubes. */
static void dfs(int depth, int start, idx_t sum,
                const idx_t *cubes, unsigned int *cnt,
                idx_t limit, int max_val)
{
    if (depth == 6) {
        if (sum <= limit)
            ++cnt[sum];
        return;
    }
    for (int v = start; v <= max_val; ++v) {
        idx_t ns = sum + cubes[v];
        if (ns > limit)
            break;
        dfs(depth + 1, v, ns, cubes, cnt, limit, max_val);
    }
}

seq_t a(idx_t n)
{
    if (!initialized) {
        /* Precompute the cube values. */
        int max_val = 0;
        while ((idx_t)(max_val + 1) * (max_val + 1) * (max_val + 1) <= MAX_LIMIT)
            ++max_val;

        idx_t *cubes = (idx_t *)malloc((max_val + 1) * sizeof(idx_t));
        if (!cubes) return 0;
        for (int i = 1; i <= max_val; ++i)
            cubes[i] = (idx_t)i * (idx_t)i * (idx_t)i;

        /* Count representations. */
        unsigned int *cnt = (unsigned int *)calloc((size_t)MAX_LIMIT + 1,
                                                    sizeof(unsigned int));
        if (!cnt) {
            free(cubes);
            return 0;
        }
        dfs(0, 1, 0, cubes, cnt, MAX_LIMIT, max_val);

        /* Determine how many sums have exactly three representations. */
        size_t tc = 0;
        for (idx_t s = 1; s <= MAX_LIMIT; ++s)
            if (cnt[s] == 3)
                ++tc;

        term_count = tc;
        if (term_count > 0) {
            terms = (seq_t *)malloc(term_count * sizeof(seq_t));
            if (!terms) {
                free(cnt);
                free(cubes);
                return 0;
            }
            size_t idx = 0;
            for (idx_t s = 1; s <= MAX_LIMIT; ++s)
                if (cnt[s] == 3)
                    terms[idx++] = (seq_t)s;
        }

        /* Clean up. */
        free(cnt);
        free(cubes);

        initialized = true;
    }

    if (n == 0 || n > term_count)
        return 0;

    return terms[(size_t)(n - 1)];
}
