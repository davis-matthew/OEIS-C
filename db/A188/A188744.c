/*
 * A188744  Number of nX6 binary arrays without the pattern 1 1 1 diagonally,
 *          vertically or antidiagonally.
 * https://oeis.org/A188744
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 26,
 *            big any n
 * Helpers:   alloc_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A188744
   Number of n×6 binary arrays without a 3‑cell 1‑pattern
   vertically, on the main diagonal or on the anti‑diagonal.
   Computed by a linear DP over pairs of consecutive rows.
*/
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/alloc_seq_array.h"   /* alloc_seq_array(size_t) */

/* Pre‑computed transition table */
static bool trans_built = false;
static uint8_t  next_cnt[4096];
static int      next_row[4096][64];

/* Check that adding a new row does not create three 1s in any direction */
static bool triple_ok(int prev2, int prev1, int cur)
{
    /* vertical */
    if ((prev2 & prev1 & cur) != 0) return false;

    /* main diagonal: prev2[j], prev1[j+1], cur[j+2]  (0<=j<=3) */
    for (int j = 0; j <= 3; ++j)
        if (((prev2 >> j) & 1) && ((prev1 >> (j+1)) & 1) && ((cur >> (j+2)) & 1))
            return false;

    /* anti‑diagonal: prev2[j], prev1[j-1], cur[j-2]  (2<=j<6) */
    for (int j = 2; j < 6; ++j)
        if (((prev2 >> j) & 1) && ((prev1 >> (j-1)) & 1) && ((cur >> (j-2)) & 1))
            return false;

    return true;
}

/* Build the transition table once */
static void build_transitions(void)
{
    if (trans_built) return;
    for (int p = 0; p < 4096; ++p) {
        int prev2 = p >> 6;      /* high 6 bits   */
        int prev1 = p & 63;      /* low  6 bits   */
        uint8_t cnt = 0;
        for (int r = 0; r < 64; ++r) {
            if (triple_ok(prev2, prev1, r))
                next_row[p][cnt++] = r;
        }
        next_cnt[p] = cnt;
    }
    trans_built = true;
}

/* Dynamic programming arrays */
static seq_t *dp_cur  = NULL;
static seq_t *dp_next = NULL;
static idx_t  curr_n  = 0;   /* largest n already computed */

/* Cache for a(n) values (index 0 holds a(1)) */
static seq_t *cache   = NULL;
static size_t cache_cap = 0;

/* Ensure cache has space for at least n entries (0‑based). */
static void ensure_cache(idx_t n)
{
    if ((size_t)n <= cache_cap) return;
    size_t new_cap = (size_t)n;
    seq_t *new_cache = alloc_seq_array(new_cap);
    for (size_t i = 0; i < cache_cap; ++i) new_cache[i] = cache[i];
    free(cache);
    cache = new_cache;
    cache_cap = new_cap;
}

/* Ensure DP arrays are allocated. */
static void ensure_dp_arrays(void)
{
    if (!dp_cur) {
        dp_cur  = alloc_seq_array(4096);
        dp_next = alloc_seq_array(4096);
    }
}

/* Public function required by the build. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for offset 1 */

    /* Base values */
    if (n == 1) return (seq_t)64;
    if (n == 2) return ((seq_t)64 * (seq_t)64);

    /* Initialise cache and DP if first call. */
    if (cache_cap == 0) {
        cache   = alloc_seq_array(2);
        cache[0] = (seq_t)64;
        cache[1] = ((seq_t)64 * (seq_t)64);
        cache_cap = 2;
    }

    /* Return cached result if already computed. */
    if (n <= (idx_t)(cache_cap-1) && n >= 1) return cache[n-1];

    build_transitions();
    ensure_dp_arrays();

    /* Initialise DP for n = 2 if this is the first computation. */
    if (curr_n == 0) {
        for (int i = 0; i < 4096; ++i) dp_cur[i] = (seq_t)1;
        curr_n = 2;
    }

    /* Extend cache to requested n. */
    ensure_cache(n);

    /* DP from curr_n+1 up to n. */
    for (idx_t k = curr_n + 1; k <= n; ++k) {
        /* zero dp_next */
        for (int i = 0; i < 4096; ++i) dp_next[i] = (seq_t)0;

        for (int p = 0; p < 4096; ++p) {
            seq_t val = dp_cur[p];
            if (val == 0) continue;
            int prev1 = p & 63;
            uint8_t cnt = next_cnt[p];
            for (int t = 0; t < cnt; ++t) {
                int r = next_row[p][t];
                int q = (prev1 << 6) | r;
                dp_next[q] += val;
            }
        }

        /* Sum all states to get a(k) */
        seq_t sum = (seq_t)0;
        for (int i = 0; i < 4096; ++i) sum += dp_next[i];
        cache[k-1] = sum;

        /* swap dp_cur and dp_next */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    curr_n = n;
    return cache[n-1];
}
