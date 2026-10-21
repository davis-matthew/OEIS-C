/*
 * A089938  Number of 7 X n matrices with entries {0,1} without adjacent 0's
 *          in any row or column. 7th row of A089934.
 * https://oeis.org/A089938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 7×n binary matrices with no adjacent 0's
 *            horizontally or vertically (i.e. the number of independent
 *            vertex sets in the grid graph P_7 × P_n). Computed by dynamic
 *            programming over column masks
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Constants */
#define HEIGHT          7
#define ALL_MASKS       (1ULL << HEIGHT)   /* mask range 0..2^7-1 */
#define MAX_VALID_MASKS 34                 /* Fibonacci number F(9) */

/* Valid column masks (no two adjacent 1's vertically). */
static idx_t  valid_masks[MAX_VALID_MASKS];
static size_t valid_count = 0;

/* Two DP tables reused for every call. */
static seq_t *dp_prev = NULL;
static seq_t *dp_curr = NULL;

/* Cache of previously computed a(n).  0 means not yet computed
   (a(n) is always >0 for n≥1). */
static seq_t *cache      = NULL;
static size_t cache_size = 0;

/* Initialise tables once. */
static void init_once(void)
{
    static bool init_done = false;
    if (init_done) return;
    init_done = true;

    /* Enumerate all masks without adjacent 1's. */
    valid_count = 0;
    for (idx_t mask = 0; mask < ALL_MASKS; ++mask) {
        if ((mask & (mask << 1)) == 0) {
            valid_masks[valid_count++] = mask;
        }
    }

    /* Allocate DP arrays. */
    dp_prev = (seq_t *)malloc(valid_count * sizeof(seq_t));
    dp_curr = (seq_t *)malloc(valid_count * sizeof(seq_t));
    if (dp_prev && dp_curr) {
        memset(dp_prev, 0, valid_count * sizeof(seq_t));
        memset(dp_curr, 0, valid_count * sizeof(seq_t));
    }
}

/* Ensure cache large enough to hold index n. */
static void ensure_cache(idx_t n)
{
    if (cache_size >= (size_t)(n + 1)) return;

    size_t new_size = (size_t)(n + 1);
    seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
    if (!new_cache) return;   /* allocation failure – unlikely for n ≤ 10000 */

    memset(new_cache, 0, new_size * sizeof(seq_t));

    if (cache) {
        memcpy(new_cache, cache, cache_size * sizeof(seq_t));
        free(cache);
    }

    cache      = new_cache;
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset starts at 1 */

    init_once();
    ensure_cache(n);

    if (cache[n] != (seq_t)0) return cache[n];

    /* First column: any valid mask is allowed. */
    for (size_t i = 0; i < valid_count; ++i)
        dp_prev[i] = (seq_t)1;

    /* Transition for columns 2…n. */
    for (idx_t col = 1; col < n; ++col) {
        for (size_t i = 0; i < valid_count; ++i) {
            seq_t sum = (seq_t)0;
            idx_t mask_i = valid_masks[i];
            for (size_t j = 0; j < valid_count; ++j) {
                if ((mask_i & valid_masks[j]) == 0)
                    sum += dp_prev[j];
            }
            dp_curr[i] = sum;
        }
        /* swap tables for next iteration */
        seq_t *tmp = dp_prev;
        dp_prev = dp_curr;
        dp_curr = tmp;
    }

    /* Sum over all possible ending masks. */
    seq_t result = (seq_t)0;
    for (size_t i = 0; i < valid_count; ++i)
        result += dp_prev[i];

    cache[n] = result;   /* memoise */
    return result;
}
