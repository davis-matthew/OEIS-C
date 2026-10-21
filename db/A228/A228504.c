/*
 * A228504  Number of n X 6 binary arrays with top left value 1 and no two
 *          ones adjacent horizontally, vertically, diagonally or
 *          antidiagonally.
 * https://oeis.org/A228504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A228504.
   The algorithm uses dynamic programming over the six columns.
   For each row we keep a 6‑bit mask of the cells that are 1.
   Masks must have no adjacent 1s horizontally.
   Two successive masks must have no overlapping 1s vertically or
   diagonally (including anti‑diagonal). 
   The top‑left cell (row 1, column 0) is forced to 1, so the
   first row only uses masks with bit 0 set.
   The number of states is 21.  The recurrence is:
       a(1) = 8
       a(n) = sum_{mask where bit0 set} dp1[mask]   (row 1)
       for k ≥ 2:
           dp_k[mask'] = Σ_{mask} dp_{k-1}[mask]  if masks compatible
           a(k) = Σ_{mask'} dp_k[mask'] */
#include <stdlib.h>
#include <stddef.h>   /* for size_t, NULL */

static seq_t *cumulative = NULL;  /* cumulative[n-1] = a(n) */
static size_t  cum_cap = 0, cum_len = 0;
static seq_t *dp_prev = NULL, *dp_next = NULL;
static size_t dp_rows = 0;
static int    valid_masks[64];
static size_t mask_cnt = 0;

/* Ensure cumulative array has capacity at least 'need' */
static void ensure_cumulative(size_t need)
{
    if (cum_cap >= need)
        return;
    size_t new_cap = cum_cap ? cum_cap << 1 : need;
    if (new_cap < need)
        new_cap = need;
    seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!tmp)
        abort();               /* allocation failure */
    for (size_t i = 0; i < cum_len; ++i)
        tmp[i] = cumulative[i];
    free(cumulative);
    cumulative = tmp;
    cum_cap = new_cap;
}

/* Enumerate all 6‑bit masks with no adjacent 1s and allocate dp arrays */
static void init_state(void)
{
    if (mask_cnt)
        return;                 /* already initialized */

    /* valid masks: no adjacent 1s horizontally */
    for (int m = 0; m < 64; ++m)
        if ((m & (m << 1)) == 0)
            valid_masks[mask_cnt++] = m;

    dp_rows = mask_cnt;
    dp_prev = (seq_t *)malloc(dp_rows * sizeof(seq_t));
    dp_next = (seq_t *)malloc(dp_rows * sizeof(seq_t));
    if (!dp_prev || !dp_next)
        abort();
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* offset is 1, but return 0 for n==0 */

    size_t sn = (size_t)n;
    ensure_cumulative(sn);
    init_state();

    /* If already computed, return */
    if (sn <= cum_len)
        return cumulative[sn - 1];

    /* Compute rows up to n */
    for (size_t row = cum_len + 1; row <= sn; ++row)
    {
        /* initialize dp_next to zero */
        for (size_t i = 0; i < dp_rows; ++i)
            dp_next[i] = 0;

        if (row == 1)
        {
            /* first row: only masks with bit 0 set */
            for (size_t i = 0; i < dp_rows; ++i)
            {
                int mask = valid_masks[i];
                dp_next[i] = (mask & 1) ? 1 : 0;
            }
        }
        else
        {
            /* transition from dp_prev to dp_next */
            for (size_t i = 0; i < dp_rows; ++i)
            {
                seq_t val = dp_prev[i];
                if (!val)
                    continue;
                int maski = valid_masks[i];
                for (size_t j = 0; j < dp_rows; ++j)
                {
                    int maskj = valid_masks[j];
                    /* no overlap vertically or diagonally (including anti-diagonal) */
                    if ((maski & maskj) == 0 &&
                        ((maski << 1) & maskj) == 0 &&
                        ((maski >> 1) & maskj) == 0)
                    {
                        dp_next[j] += val;
                    }
                }
            }
        }

        /* compute sum for this row */
        seq_t sum = 0;
        for (size_t i = 0; i < dp_rows; ++i)
            sum += dp_next[i];

        cumulative[row - 1] = sum;

        /* swap dp_prev and dp_next for next iteration */
        seq_t *tmp = dp_prev;
        dp_prev = dp_next;
        dp_next = tmp;

        cum_len = row;
    }

    return cumulative[sn - 1];
}
