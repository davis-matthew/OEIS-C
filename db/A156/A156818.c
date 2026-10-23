/*
 * A156818  Last digit d of a(n) is present in a(n+1+d); a(1)=1; a(n)<a(n+1).
 * https://oeis.org/A156818
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(1)=1, a(n)<a(n+1). For each n, let d be the last * digit of
 *            a(n); then a(n+1+d) must contain digit d. If there is no * duty
 *            for a(n) to contain any digits, a(n)=a(n−1)+1. The sequence * is
 *            computed by keeping a forward demand table of required digits
 * Range:     u8 n <= 25, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/ensure_capacity.h"

static seq_t      *a_vals   = NULL;   /* array of computed values            */
static size_t     a_cap    = 0;       /* capacity of a_vals                  */
static size_t     a_len    = 0;       /* number of values actually computed   */

static uint16_t   *dem_mask = NULL;   /* bitmask of demanded digits for each
                                      * future position: bit d set iff digit d
                                      * must appear in that a(n)            */
static size_t     dem_cap  = 0;       /* capacity of dem_mask                */
static size_t     dem_len  = 0;       /* index of first unused mask position */

/* Test whether cand contains all digits required by mask. */
static bool candidate_ok(seq_t cand, uint16_t mask)
{
    if (mask == 0u)
        return true;

    seq_t tmp = cand;
    while (tmp > 0) {
        int d = (int)(tmp % 10);
        mask &= (uint16_t)~(1u << d);
        if (mask == 0u)
            return true;
        tmp /= 10;
    }
    return mask == 0u;
}

seq_t a(idx_t n)
{
    if (n == 0)            /* offset starts at 1, but callers never use 0 */
        return 0;

    size_t target = (size_t)n;          /* number of terms required */

    /* Grow a_vals so it can hold target terms */
    if (a_cap < target) {
        ensure_capacity(target, &a_cap, (void **)&a_vals, sizeof(seq_t));
    }

    /* Lazy initialization of first term and its demand */
    if (a_len == 0) {
        a_vals[0] = (seq_t)1;
        a_len = 1;

        /* a(1) ends with digit 1 → constraint on a(3) (index 2) */
        if (dem_cap < 3u) {
            ensure_capacity(3u, &dem_cap, (void **)&dem_mask, sizeof(uint16_t));
            for (size_t i = 0; i < dem_cap; ++i)
                dem_mask[i] = 0;
        }
        dem_mask[2] |= (1u << 1);   /* digit 1 required in a(3) */
        dem_len = 3u;
    }

    /* Compute terms up to target */
    for (size_t idx = a_len; idx < target; ++idx) {
        seq_t cand = a_vals[idx - 1] + 1;
        uint16_t mask = 0;
        if (idx < dem_len)
            mask = dem_mask[idx];

        while (!candidate_ok(cand, mask))
            cand += 1;

        a_vals[idx] = cand;
        if (idx < dem_len)
            dem_mask[idx] = 0;   /* demand satisfied */

        /* Add new demand based on last digit of cand */
        unsigned dlast = (unsigned)(cand % 10);
        size_t future_idx = idx + 1 + dlast;   /* 0‑based index for a(n+1+d) */

        if (dem_cap <= future_idx) {
            size_t new_cap = future_idx + 20u;
            ensure_capacity(new_cap, &dem_cap, (void **)&dem_mask,
                            sizeof(uint16_t));
            for (size_t i = dem_len; i < dem_cap; ++i)
                dem_mask[i] = 0;
        }
        if (future_idx >= dem_len)
            dem_len = future_idx + 1;

        dem_mask[future_idx] |= (1u << dlast);
    }

    a_len = target;
    return a_vals[target - 1];
}
