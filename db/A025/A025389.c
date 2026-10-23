/*
 * A025389  Numbers that are the sum of 4 distinct nonzero squares in 5 or
 *          more ways.
 * https://oeis.org/A025389
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: sum of 4 distinct non‑zero squares in ≥5 ways
 *
 * The algorithm builds a count array for the number of ways to represent a
 * sum as the sum of 4 distinct squares using a dynamic programming
 * recurrence.  It stores all sums that have at least 5 representations
 * in a static cache.  The cache is rebuilt only when a requested index
 * exceeds the currently prepared terms.
 */
#include <stdlib.h>
#include <stdbool.h>

static idx_t  *cache        = NULL;   /* cached sequence values          */
static size_t  cache_len    = 0;      /* number of cached terms          */
static size_t  cache_cap    = 0;      /* allocated capacity of cache     */
static idx_t   current_max  = 0;      /* highest sum for which cache built */

static void rebuild_cache(idx_t max_sum)
{
    /* free previous cache */
    if (cache) free(cache);
    cache = NULL;
    cache_len = 0;
    cache_cap = 0;
    current_max = max_sum;

    /* compute maximum root such that i*i <= max_sum */
    idx_t max_root = 0;
    while ((max_root + 1) * (max_root + 1) <= max_sum) ++max_root;

    /* allocate count arrays (uint8_t enough, clamp to 5) */
    uint8_t *cnt1 = (uint8_t*)calloc((size_t)(max_sum + 1), sizeof(uint8_t));
    uint8_t *cnt2 = (uint8_t*)calloc((size_t)(max_sum + 1), sizeof(uint8_t));
    uint8_t *cnt3 = (uint8_t*)calloc((size_t)(max_sum + 1), sizeof(uint8_t));
    uint8_t *cnt4 = (uint8_t*)calloc((size_t)(max_sum + 1), sizeof(uint8_t));
    if (!cnt1 || !cnt2 || !cnt3 || !cnt4) {
        exit(EXIT_FAILURE);
    }

    size_t current_max_sum = 0;   /* max sum attainable up to current square */

    for (idx_t i = 1; i <= max_root; ++i) {
        idx_t sq = i * i;                    /* current square value     */
        size_t prev_max = current_max_sum;   /* previous attainable max  */
        current_max_sum += sq;
        if (current_max_sum > (size_t)max_sum) current_max_sum = (size_t)max_sum;

        /* ----- update cnt4 from cnt3 --------------------------------- */
        for (size_t sum = current_max_sum; sum >= (size_t)sq; --sum) {
            size_t idx = sum - sq;
            if (idx > prev_max) continue;
            uint8_t add = cnt3[idx];
            if (!add) continue;
            uint8_t newv = cnt4[sum] + add;
            cnt4[sum] = newv > 5 ? 5 : newv;
        }

        /* ----- update cnt3 from cnt2 --------------------------------- */
        for (size_t sum = current_max_sum; sum >= (size_t)sq; --sum) {
            size_t idx = sum - sq;
            if (idx > prev_max) continue;
            uint8_t add = cnt2[idx];
            if (!add) continue;
            uint8_t newv = cnt3[sum] + add;
            cnt3[sum] = newv > 5 ? 5 : newv;
        }

        /* ----- update cnt2 from cnt1 --------------------------------- */
        for (size_t sum = current_max_sum; sum >= (size_t)sq; --sum) {
            size_t idx = sum - sq;
            if (idx > prev_max) continue;
            uint8_t add = cnt1[idx];
            if (!add) continue;
            uint8_t newv = cnt2[sum] + add;
            cnt2[sum] = newv > 5 ? 5 : newv;
        }

        /* mark presence of a single square */
        if (sq <= max_sum) cnt1[sq] = 1;
    }

    /* collect all qualifying sums into cache */
    size_t cap = 1024;
    cache = (idx_t*)malloc(cap * sizeof(idx_t));
    if (!cache) exit(EXIT_FAILURE);

    for (size_t sum = 1; sum <= (size_t)max_sum; ++sum) {
        if (cnt4[sum] >= 5) {
            if (cache_len == cap) {
                cap <<= 1;
                idx_t *tmp = (idx_t*)realloc(cache, cap * sizeof(idx_t));
                if (!tmp) exit(EXIT_FAILURE);
                cache = tmp;
            }
            cache[cache_len++] = (idx_t)sum;
        }
    }
    cache_cap = cap;

    free(cnt1);
    free(cnt2);
    free(cnt3);
    free(cnt4);
}

seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) {
        rebuild_cache(500000);   /* initial bound giving many terms   */
        initialized = true;
    }

    if (n == 0) return (seq_t)0;   /* undefined for n=0 but safe return */

    /* extend cache if needed */
    if ((size_t)n > cache_len) {
        idx_t new_max = current_max * 2;
        if (new_max <= current_max) new_max = current_max + 1;
        rebuild_cache(new_max);
        if ((size_t)n > cache_len) return (seq_t)0;   /* still not enough */
    }

    return (seq_t)cache[n - 1];
}
