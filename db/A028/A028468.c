/*
 * A028468  Number of perfect matchings in graph P_{6} X P_{n}.
 * https://oeis.org/A028468
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of perfect matchings in P₆ × Pₙ  (OEIS A028468)
   Computed by transfer‑matrix dynamic programming for fixed width 6.
*/

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* transition matrix T[s][t] = number of ways to go from state s
   to state t when filling one column of height 6 */
static seq_t T[64][64] = {0};

/* Recursive helper that enumerates all tile placements for a
   given incoming state s_orig.  It calls itself with the next rows
   to be processed, building the outgoing mask 'next'. */
static void rec_helper(int s_orig, int row, int next)
{
    if (row == 6) {
        /* all rows processed: record a transition from s_orig to next */
        T[s_orig][next] += 1;
        return;
    }
    if (s_orig & (1 << row)) {
        /* this cell already occupied by a horizontal domino coming from left */
        rec_helper(s_orig, row + 1, next);
    } else {
        /* try a vertical domino (must fit) */
        if (row + 1 < 6 && !(s_orig & (1 << (row + 1)))) {
            rec_helper(s_orig, row + 2, next);
        }
        /* try a horizontal domino to the next column */
        rec_helper(s_orig, row + 1, next | (1 << row));
    }
}

/* Build the transition matrix once */
static void build_trans(void)
{
    static int built = 0;
    if (built) return;
    for (int s = 0; s < 64; ++s) {
        rec_helper(s, 0, 0);
    }
    built = 1;
}

/* Dynamic‑programming buffers for current and next column */
static seq_t dp_cur[64];
static seq_t dp_next[64];

/* Number of columns already processed */
static idx_t computed_n = 0;
/* Flag whether DP has been initialized for n = 0 */
static int dp_initialized = 0;

/* Cache of computed terms a(0)..a(computed_n) */
static seq_t *cache = NULL;
static size_t cache_cap = 0;

/* Ensure that the cache contains results up to index n */
static void ensure_cache(idx_t n)
{
    build_trans();

    /* enlarge cache if necessary */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();
        /* initialise new slots to 0 (they will soon be overwritten) */
        for (size_t i = cache_cap; i < new_cap; ++i) new_cache[i] = 0;
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* initialise DP for first call */
    if (!dp_initialized) {
        for (int i = 0; i < 64; ++i) dp_cur[i] = 0;
        dp_cur[0] = 1;
        cache[0] = 1;
        computed_n = 0;
        dp_initialized = 1;
    }

    /* extend DP up to n */
    for (idx_t col = computed_n + 1; col <= n; ++col) {
        /* clear next buffer */
        for (int i = 0; i < 64; ++i) dp_next[i] = 0;

        /* transition */
        for (int s = 0; s < 64; ++s) {
            seq_t cur_val = dp_cur[s];
            if (cur_val == 0) continue;
            for (int t = 0; t < 64; ++t) {
                seq_t ways = T[s][t];
                if (ways == 0) continue;
                dp_next[t] += cur_val * ways;
            }
        }

        /* store result for this column */
        cache[(size_t)col] = dp_next[0];

        /* prepare for next iteration */
        for (int i = 0; i < 64; ++i) dp_cur[i] = dp_next[i];
    }

    computed_n = n;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
