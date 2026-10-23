/*
 * A164160  Number of binary strings of length n with equal numbers of 0001
 *          and 0111 substrings.
 * https://oeis.org/A164160
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of binary strings of length n with equal numbers *
 *            of substrings 0001 and 0111
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 34, u64 n <= 66, u128 n <= 131,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <string.h>

/* maximum n we will handle – adjust if larger n are required */
#ifndef MAX_N
#define MAX_N 20000
#endif

#define OFFSET    MAX_N
#define DIFF_SIZE (2 * MAX_N + 1)

/* static state used for memoisation */
static seq_t *dp_cur  = NULL;   /* counts for current length   */
static seq_t *dp_next = NULL;   /* counts for next length      */
static seq_t *ans     = NULL;   /* a(n) values                */
static idx_t  cur_len = 0;      /* length already computed    */
static bool   init    = false;

/* initialise static arrays once */
static void init_once(void)
{
    if (init) return;
    const size_t total_cells = 8 * DIFF_SIZE;
    dp_cur  = (seq_t *)malloc(total_cells * sizeof(seq_t));
    dp_next = (seq_t *)malloc(total_cells * sizeof(seq_t));
    ans     = (seq_t *)malloc((MAX_N + 1) * sizeof(seq_t));
    if (!dp_cur || !dp_next || !ans) return;  /* allocation failed */

    memset(dp_cur,  0, total_cells * sizeof(seq_t));
    memset(dp_next, 0, total_cells * sizeof(seq_t));

    /* a(0) = 1 */
    dp_cur[0 * DIFF_SIZE + OFFSET] = 1;
    ans[0] = 1;
    cur_len = 0;
    init = true;
}

/* extend the precomputation up to length n */
static void extend_to(idx_t n)
{
    while (cur_len < n) {
        idx_t new_len = cur_len + 1;

        /* ------- zero dp_next for the new length ------- */
        size_t diff_new  = (new_len >= 3) ? (new_len - 3) : 0;
        size_t start_new = OFFSET - diff_new;
        size_t end_new   = OFFSET + diff_new;
        size_t width_new = end_new - start_new + 1;

        for (size_t m = 0; m < 8; ++m) {
            memset(&dp_next[m * DIFF_SIZE + start_new],
                   0,
                   width_new * sizeof(seq_t));
        }

        /* ------- transition from current length ------- */
        size_t diff_cur = (cur_len >= 3) ? (cur_len - 3) : 0;
        size_t start_cur = OFFSET - diff_cur;
        size_t end_cur   = OFFSET + diff_cur;

        for (size_t mask = 0; mask < 8; ++mask) {
            size_t base_cur  = mask * DIFF_SIZE;
            size_t base_next = mask * DIFF_SIZE;  /* mask shifts in the loop */

            for (size_t idx = start_cur; idx <= end_cur; ++idx) {
                seq_t cnt = dp_cur[base_cur + idx];
                if (cnt == 0) continue;

                /* new masks */
                size_t new_mask0 = (mask << 1) & 7;   /* append 0 */
                size_t new_mask1 = new_mask0 | 1;     /* append 1 */

                /* compute inc for substring 0001 (+1) and 0111 (-1) */
                int inc0 = 0, inc1 = 0;
                if (cur_len >= 3) {
                    int new4_0 = ((mask << 1) & 0xF);
                    if (new4_0 == 0b0001) inc0 =  1;
                    else if (new4_0 == 0b0111) inc0 = -1;

                    int new4_1 = ((mask << 1) | 1) & 0xF;
                    if (new4_1 == 0b0001) inc1 =  1;
                    else if (new4_1 == 0b0111) inc1 = -1;
                }

                /* new diff indices */
                int idx0_val = (int)idx + inc0;
                int idx1_val = (int)idx + inc1;
                size_t idx0 = (size_t)idx0_val;
                size_t idx1 = (size_t)idx1_val;

                /* add counts */
                dp_next[new_mask0 * DIFF_SIZE + idx0] += cnt;
                dp_next[new_mask1 * DIFF_SIZE + idx1] += cnt;
            }
        }

        /* swap current and next DP tables */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;

        /* update length and record a[new_len] */
        cur_len = new_len;
        seq_t sum = 0;
        for (size_t m = 0; m < 8; ++m) {
            sum += dp_cur[m * DIFF_SIZE + OFFSET];
        }
        ans[cur_len] = sum;
    }
}

seq_t a(idx_t n)
{
    init_once();
    if (n > MAX_N) /* n beyond allocated range – treat as 0 or grow here */
        return 0;
    extend_to(n);
    return ans[n];
}
