/*
 * A236519  Number of (n+1)X(4+1) 0..1 arrays with the difference of the upper
 *          median and minimum value of each 2X2 subblock in lexicographically
 *          nondecreasing order columnwise and nonincreasing rowwise.
 * https://oeis.org/A236519
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 7, u64 n <= 17, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A236519 – number of (n+1)×5 0/1 arrays with the difference of the
 *
 *  upper median and minimum value of each 2×2 subblock
 *  in lexicographically nondecreasing order columnwise
 *  and nonincreasing rowwise.
 *
 *  This implementation uses dynamic programming over the rows.
 *  The state is the last row (5 bits) and the 4‑bit mask that records
 *  which of the 4 subblocks (columns 0–1,1–2,2–3,3–4) had a
 *  “difference” of 1 (i.e. the subblock contains 2 or 3 ones).
 *
 *  For a row pair the difference mask must satisfy two monotonicity
 *  constraints:
 *
 *      • each row of the difference matrix is nondecreasing
 *        left → right  →  allowed masks are 0000,0001,0011,0111,1111.
 *
 *      • the difference sequence down the rows is nonincreasing
 *        lexicographically – this is equivalent to
 *        for consecutive masks  prev & ~curr == 0.
 *
 *  The DP starts with all pairs of rows for n = 1.
 *  Subsequent rows are added by the transition described above.
 *
 *  The result is memoised in a cache so that a( n ) can be called
 *  in any order.
 */

#include <stdlib.h>
#include <stdbool.h>

/* helper tables – static, global */
static uint8_t diffMask[32][32];          /* difference mask of a row pair      */
static int    mask_map[16];               /* -1 if mask not allowed, else 0..4  */
static const uint8_t allowed_masks[5] = {0,1,3,7,15};

static seq_t (*dp_cur)[16];
static seq_t (*dp_next)[16];
static seq_t buffer0[32][16];
static seq_t buffer1[32][16];

static seq_t *a_cache = NULL;            /* memoised sequence values          */
static idx_t  cache_cap = 0;
static idx_t  computed_up_to = 0;        /* largest n for which a_cache[n] is valid */

/* build the difference mask table and the mask → index mapping */
static void init_globals(void)
{
    /* difference masks */
    for (int r0 = 0; r0 < 32; ++r0) {
        for (int r1 = 0; r1 < 32; ++r1) {
            uint8_t mask = 0;
            for (int j = 0; j < 4; ++j) {
                int s = ((r0 >> j) & 1) + ((r0 >> (j + 1)) & 1)
                      + ((r1 >> j) & 1) + ((r1 >> (j + 1)) & 1);
                if (s == 2 || s == 3) mask |= (1u << j);
            }
            diffMask[r0][r1] = mask;
        }
    }

    /* mapping from mask to 0..4 index (or -1 if not allowed) */
    for (int m = 0; m < 16; ++m) {
        int idx = -1;
        for (int k = 0; k < 5; ++k)
            if (m == allowed_masks[k]) { idx = k; break; }
        mask_map[m] = idx;
    }

    dp_cur  = buffer0;
    dp_next = buffer1;

    /* initialise DP for n = 1  (arrays of 2 rows)       */
    for (int r0 = 0; r0 < 32; ++r0) {
        for (int r1 = 0; r1 < 32; ++r1) {
            uint8_t m = diffMask[r0][r1];
            if (mask_map[m] >= 0) {
                dp_cur[r1][m] += (seq_t)1;
            }
        }
    }

    /* compute a(1) by summing all valid states */
    seq_t sum = 0;
    for (int r = 0; r < 32; ++r) {
        for (int m = 0; m < 16; ++m)
            if (mask_map[m] >= 0)
                sum += dp_cur[r][m];
    }

    a_cache = (seq_t *)malloc( (size_t)2 * sizeof(seq_t) );
    a_cache[0] = (seq_t)0;    /* unused, offset 1 */
    a_cache[1] = sum;
    cache_cap  = 2;
    computed_up_to = 1;
}

/* ensure the cache array can hold index up to n */
static void ensure_cache(idx_t n)
{
    if (n >= cache_cap) {
        size_t newcap = (size_t)n + 1;
        a_cache = (seq_t *)realloc(a_cache, newcap * sizeof(seq_t));
        cache_cap = newcap;
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    static bool initialized = false;
    if (!initialized) { init_globals(); initialized = true; }

    ensure_cache(n);

    if (computed_up_to >= n) return a_cache[n];

    /* extend the DP layer by layer until n is reached */
    for (idx_t target = computed_up_to + 1; target <= n; ++target) {
        /* zero the next buffer */
        for (int r = 0; r < 32; ++r)
            for (int m = 0; m < 16; ++m)
                dp_next[r][m] = (seq_t)0;

        /* transition from current to next row */
        for (int r_prev = 0; r_prev < 32; ++r_prev) {
            for (int prevM = 0; prevM < 16; ++prevM) {
                if (mask_map[prevM] < 0) continue;          /* only allowed masks */
                const seq_t curCnt = dp_cur[r_prev][prevM];
                if (curCnt == (seq_t)0) continue;

                for (int r_next = 0; r_next < 32; ++r_next) {
                    uint8_t m = diffMask[r_prev][r_next];
                    if (mask_map[m] < 0) continue;          /* not allowed */

                    /* columnwise nondecreasing (subset) */
                    if ((prevM & (uint8_t)~m) != 0) continue;

                    dp_next[r_next][m] += curCnt;
                }
            }
        }

        /* compute a(target) from dp_next */
        seq_t sum = 0;
        for (int r = 0; r < 32; ++r)
            for (int m = 0; m < 16; ++m)
                if (mask_map[m] >= 0)
                    sum += dp_next[r][m];

        a_cache[target] = sum;

        /* swap buffers for next iteration */
        seq_t (*tmp)[16] = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    computed_up_to = n;
    return a_cache[n];
}
