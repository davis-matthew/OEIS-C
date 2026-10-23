/*
 * A189005  Number of domino tilings of the 9 X n grid with upper left corner
 *          removed iff n is odd.
 * https://oeis.org/A189005
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of domino tilings of a 9×n grid with upper-left
 *            corner removed iff n is odd
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/ensure_cache.h"

/* Transition tables for the 9×1 column (height = 9). */
#define MAX_TRANS 256
static uint16_t trans_nextmask[512][MAX_TRANS];
static uint16_t trans_count  [512][MAX_TRANS];
static size_t   trans_len    [512];
static bool     trans_done   = false;
static uint16_t tmp_counts[512];

/* Recursively enumerate all ways to fill a column given an initial mask. */
static void dfs_fill(uint16_t used, uint16_t nextMask)
{
    int r = 0;
    while (r < 9 && (used & (1 << r))) ++r;
    if (r == 9) {                         /* column fully covered */
        ++tmp_counts[nextMask];
        return;
    }
    /* Place a vertical domino if possible. */
    if (r + 1 < 9 && !(used & (1 << (r + 1)))) {
        dfs_fill(used | (1 << r) | (1 << (r + 1)), nextMask);
    }
    /* Place a horizontal domino, leaving the cell in the next column. */
    dfs_fill(used | (1 << r), nextMask | (1 << r));
}

/* Build transition tables once. */
static void generate_transitions(void)
{
    for (uint16_t mask = 0; mask < 512; ++mask) {
        memset(tmp_counts, 0, sizeof tmp_counts);
        dfs_fill(mask, 0);
        size_t len = 0;
        for (uint16_t nm = 0; nm < 512; ++nm) if (tmp_counts[nm]) {
            trans_nextmask[mask][len] = nm;
            trans_count[mask][len]    = tmp_counts[nm];
            ++len;
        }
        trans_len[mask] = len;
    }
    trans_done = true;
}

/* The required sequence function. */
seq_t a(idx_t n)
{
    static bool initialized = false;
    static size_t computed_max = 0;   /* largest n for which cache[n] is filled */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;

    static seq_t dp_even[512];
    static seq_t dp_odd[512];
    static seq_t next_even[512];
    static seq_t next_odd[512];

    if (!initialized) {
        if (!trans_done) generate_transitions();

        /* initialise DP states for width 0 */
        memset(dp_even, 0, sizeof dp_even);
        memset(dp_odd, 0, sizeof dp_odd);
        dp_even[0] = 1;       /* even width starts with mask 0 */
        dp_odd[1]  = 1;       /* odd width starts with mask 1 (hole at row 0) */

        /* initialise cache for a(0) */
        cache_cap = 1;
        cache = (seq_t*)malloc(sizeof(seq_t));
        cache[0] = 1;          /* a(0) = 1 */
        computed_max = 0;
        initialized = true;
    }

    if (n <= computed_max) return cache[n];

    /* Ensure capacity for needed values */
    ensure_cache(n, &cache, &cache_cap);

    for (size_t col = computed_max; col < n; ++col) {
        /* compute next DP states for both parities */
        memset(next_even, 0, sizeof next_even);
        memset(next_odd, 0, sizeof next_odd);

        for (int cur = 0; cur < 512; ++cur) {
            if (dp_even[cur]) {
                size_t len = trans_len[cur];
                const uint16_t *nm = trans_nextmask[cur];
                const uint16_t *cnt = trans_count[cur];
                seq_t val = dp_even[cur];
                for (size_t t = 0; t < len; ++t) {
                    next_even[nm[t]] += val * (seq_t)cnt[t];
                }
            }
            if (dp_odd[cur]) {
                size_t len = trans_len[cur];
                const uint16_t *nm = trans_nextmask[cur];
                const uint16_t *cnt = trans_count[cur];
                seq_t val = dp_odd[cur];
                for (size_t t = 0; t < len; ++t) {
                    next_odd[nm[t]] += val * (seq_t)cnt[t];
                }
            }
        }

        /* bring next states to current */
        memcpy(dp_even, next_even, sizeof dp_even);
        memcpy(dp_odd, next_odd, sizeof dp_odd);

        /* store value for this width (col+1) */
        size_t idx = col + 1;
        if (idx & 1)      /* odd */
            cache[idx] = dp_odd[0];
        else              /* even */
            cache[idx] = dp_even[0];
    }

    computed_max = n;
    return cache[n];
}
