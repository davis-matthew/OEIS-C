/*
 * A197042  Number of nX3 0..4 arrays with each element x equal to the number
 *          its horizontal and vertical neighbors equal to 4,2,0,1,3 for
 *          x=0,1,2,3,4.
 * https://oeis.org/A197042
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of n×3 arrays over {0,1,2,3,4} such that
 * for each cell value x the number of horizontal and vertical neighbours
 * equal to the designated target is exactly:
 *   target(0)=4 , target(1)=2 , target(2)=0 , target(3)=1 , target(4)=3
 * This procedure computes the values by dynamic programming over rows.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

/* values used in the neighbour test */
static const int TARGET_VAL[5] = {4, 2, 0, 1, 3};
static const int REQ_COUNT[5] = {0, 1, 2, 3, 4};

/* state encoding: 3‑digit base‑5 number (0..124) × 3‑bit mask (0..7) */
#define STATES (125 * 8)

/* transition table, one array per source state, up to 125 destinations */
static uint16_t transition_dest[STATES][125];
static uint8_t transition_cnt[STATES];
static bool transition_built = false;

/* DP rows */
static seq_t *dp_cur = NULL;
static seq_t *dp_next = NULL;

/* cache of computed sequence values */
static seq_t *a_cache = NULL;
static idx_t a_cache_cap = 0;   /* allocated size */
static idx_t a_cache_len = 0;   /* last index with a value stored */

/* build the transition table once */
static void build_transitions(void)
{
    if (transition_built)
        return;

    /* pre‑compute digit decomposition of each value 0..124 */
    unsigned char dig0[125], dig1[125], dig2[125];
    for (int v = 0; v < 125; ++v) {
        dig0[v] = v / 25;
        dig1[v] = (v / 5) % 5;
        dig2[v] = v % 5;
    }

    for (int src = 0; src < STATES; ++src) {
        int prevVal = src / 8;
        int remMaskPrev = src & 7;
        int prev0 = dig0[prevVal], prev1 = dig1[prevVal], prev2 = dig2[prevVal];

        transition_cnt[src] = 0;

        for (int curVal = 0; curVal < 125; ++curVal) {
            int cur0 = dig0[curVal], cur1 = dig1[curVal], cur2 = dig2[curVal];
            bool ok = true;

            /* check vertical constraints from the row above */
            for (int j = 0; j < 3 && ok; ++j) {
                int valPrev = (j == 0) ? prev0 : (j == 1) ? prev1 : prev2;
                int targetPrev = TARGET_VAL[valPrev];
                bool mustBe = (remMaskPrev >> j) & 1;
                int valCurr = (j == 0) ? cur0 : (j == 1) ? cur1 : cur2;
                if (mustBe) {
                    if (valCurr != targetPrev)
                        ok = false;
                } else {
                    if (valCurr == targetPrev)
                        ok = false;
                }
            }
            if (!ok)
                continue;

            /* compute remaining requirement for the current row */
            int remMaskCurr = 0;
            for (int j = 0; j < 3; ++j) {
                int valCurr = (j == 0) ? cur0 : (j == 1) ? cur1 : cur2;
                int targetCurr = TARGET_VAL[valCurr];

                int hcnt = 0;
                if (j > 0) {
                    int left = (j - 1 == 0) ? cur0 : (j - 1 == 1 ? cur1 : cur2);
                    if (left == targetCurr)
                        hcnt++;
                }
                if (j < 2) {
                    int right = (j + 1 == 0) ? cur0 : (j + 1 == 1 ? cur1 : cur2);
                    if (right == targetCurr)
                        hcnt++;
                }
                int upcnt = ((j == 0) ? prev0 : (j == 1) ? prev1 : prev2) == targetCurr ? 1 : 0;
                int curCnt = hcnt + upcnt;
                int rem = REQ_COUNT[valCurr] - curCnt;
                if (rem < 0 || rem > 1)
                    { ok = false; break; }
                if (rem == 1)
                    remMaskCurr |= 1 << j;
            }
            if (!ok)
                continue;

            int dest = curVal * 8 + remMaskCurr;
            transition_dest[src][transition_cnt[src]++] = dest;
        }
    }
    transition_built = true;
}

/* compute the DP for the first row and initialise dp_cur */
static void init_first_row(void)
{
    /* pad the dp arrays if not yet allocated */
    if (!dp_cur) {
        dp_cur  = alloc_seq_array(STATES);
        dp_next = alloc_seq_array(STATES);
    }
    for (int i = 0; i < STATES; ++i)
        dp_cur[i] = 0;

    /* pre‑compute digit decomposition of each value 0..124 */
    unsigned char dig0[125], dig1[125], dig2[125];
    for (int v = 0; v < 125; ++v) {
        dig0[v] = v / 25;
        dig1[v] = (v / 5) % 5;
        dig2[v] = v % 5;
    }

    for (int curVal = 0; curVal < 125; ++curVal) {
        int cur0 = dig0[curVal], cur1 = dig1[curVal], cur2 = dig2[curVal];
        int remMask = 0;

        bool ok = true;
        for (int j = 0; j < 3 && ok; ++j) {
            int valCurr = (j == 0) ? cur0 : (j == 1) ? cur1 : cur2;
            int targetCurr = TARGET_VAL[valCurr];

            int hcnt = 0;
            if (j > 0) {
                int left = (j - 1 == 0) ? cur0 : (j - 1 == 1 ? cur1 : cur2);
                if (left == targetCurr)
                    hcnt++;
            }
            if (j < 2) {
                int right = (j + 1 == 0) ? cur0 : (j + 1 == 1 ? cur1 : cur2);
                if (right == targetCurr)
                    hcnt++;
            }
            /* no up neighbor in first row */
            int curCnt = hcnt;
            int rem = REQ_COUNT[valCurr] - curCnt;
            if (rem < 0 || rem > 1) {
                ok = false; break;
            }
            if (rem == 1)
                remMask |= 1 << j;
        }
        if (!ok)
            continue;

        int dest = curVal * 8 + remMask;
        dp_cur[dest] += 1;   /* exactly one way to obtain this state */
    }
}

/* caching helper for a_cache */
static void ensure_cache(idx_t n)
{
    if (n < a_cache_len)
        return;
    if (a_cache_cap < n + 1) {
        idx_t newCap = a_cache_cap ? a_cache_cap * 2 : 32;
        while (newCap <= n)
            newCap *= 2;
        seq_t *tmp = alloc_seq_array(newCap);
        for (idx_t i = 0; i < a_cache_len; ++i)
            tmp[i] = a_cache[i];
        free_seq_array(a_cache);
        a_cache = tmp;
        a_cache_cap = newCap;
    }
    a_cache_len = n;
}

/* main function required by the framework */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* OEIS offset starts at 1 */

    /* initialise structures only once */
    if (!transition_built)
        build_transitions();

    if (!a_cache) {
        a_cache = alloc_seq_array(32);
        a_cache_cap = 32;
        a_cache_len = 0;
        /* compute first row when needed */
    }

    /* compute up to requested n if necessary */
    if (a_cache_len < n) {
        /* initialise first row once */
        if (a_cache_len == 0) {
            init_first_row();
            /* store a(1) */
            seq_t sum = 0;
            for (int s = 0; s < STATES; ++s)
                if ((s & 7) == 0)
                    sum += dp_cur[s];
            a_cache[1] = sum;
            a_cache_len = 1;
        }

        idx_t curr = a_cache_len;   /* last row already computed */

        for (idx_t row = curr + 1; row <= n; ++row) {
            /* zero next row */
            for (int i = 0; i < STATES; ++i)
                dp_next[i] = 0;

            /* transition from dp_cur to dp_next */
            for (int s = 0; s < STATES; ++s) {
                seq_t ways = dp_cur[s];
                if (ways == 0)
                    continue;
                uint8_t pc = transition_cnt[s];
                uint16_t *dst = transition_dest[s];
                for (int k = 0; k < pc; ++k)
                    dp_next[dst[k]] += ways;
            }

            /* swap arrays */
            seq_t *tmp = dp_cur;
            dp_cur = dp_next;
            dp_next = tmp;

            /* compute a(row) as sum over states with mask==0 */
            seq_t sum = 0;
            for (int s = 0; s < STATES; ++s)
                if ((s & 7) == 0)
                    sum += dp_cur[s];

            ensure_cache(row);
            a_cache[row] = sum;
        }
    }

    return a_cache[n];
}
