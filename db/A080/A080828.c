/*
 * A080828  Triangle read by rows in which n-th row gives trajectory of n
 *          (omitting n itself) under the map k -> 3k-1 if k odd, k -> k/2 if
 *          k even, stopping when reach 1, 5 or 17.
 * https://oeis.org/A080828
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   a(n) is the n-th value in the flattened triangle of 3x‑1
 *            trajectories. For each integer s ≥ 2 the trajectory of s
 *            (omitting s itself) is formed by repeatedly applying: if k odd:
 *            k = 3k-1 if k even: k = k/2 until k becomes 1, 5, or 17. The
 *            entire triangle, read by rows in order of increasing s, is...
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* advance one step in the 3x‑1 map */
static inline seq_t next_value(seq_t v)
{
    return (v & 1) ? (seq_t)3 * v - 1 : v >> 1;
}

/* length of the trajectory for starting value n (excluding n, including
   final terminating value). */
static inline idx_t row_length(idx_t n)
{
    seq_t cur = (seq_t)n;
    idx_t len = 0;
    for (;;)
    {
        cur = next_value(cur);
        ++len;
        if (cur == 1 || cur == 5 || cur == 17)
            break;
    }
    return len;
}

/* static cache of cumulative row lengths: cum_len[i] = total terms
   up to and including row (i+2).  The array grows as required. */
static idx_t *cum_len_arr  = NULL;   /* cumulative lengths */
static size_t cum_len_cap  = 0;      /* capacity of cum_len_arr */
static size_t cum_len_cnt  = 0;      /* number of filled entries */

/* compute a(n) for n >= 2 */
seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;           /* undefined for indices < offset */

    /* extend the cache until it covers index n-1 (the offset is 2) */
    while (cum_len_cnt == 0 || cum_len_arr[cum_len_cnt - 1] < (n - 1))
    {
        idx_t start_val = (idx_t)(cum_len_cnt + 2);
        idx_t len = row_length(start_val);

        /* ensure capacity */
        if (cum_len_cnt >= cum_len_cap)
        {
            size_t new_cap = cum_len_cap ? cum_len_cap << 1 : 16;
            idx_t *tmp = (idx_t *)realloc(cum_len_arr, new_cap * sizeof(idx_t));
            if (!tmp)
                return (seq_t)0;      /* allocation failure; unlikely for tests */
            cum_len_arr = tmp;
            cum_len_cap = new_cap;
        }

        /* store cumulative length */
        if (cum_len_cnt == 0)
            cum_len_arr[0] = len;
        else
            cum_len_arr[cum_len_cnt] = cum_len_arr[cum_len_cnt - 1] + len;
        ++cum_len_cnt;
    }

    /* binary search for the row containing index n */
    size_t lo = 0, hi = cum_len_cnt - 1;
    while (lo < hi)
    {
        size_t mid = (lo + hi) >> 1;
        if (cum_len_arr[mid] >= (n - 1))
            hi = mid;
        else
            lo = mid + 1;
    }
    size_t idx = lo;                       /* index in cum_len_arr */
    idx_t  row  = (idx_t)(idx + 2);         /* starting value of the row */
    idx_t  prev = (idx == 0) ? 0 : cum_len_arr[idx - 1];
    idx_t  start_index = prev + 2;          /* first flat index of this row */
    idx_t  pos = n - start_index;           /* zero‑based offset in the row */

    /* advance pos+1 steps from the starting value to reach the required term */
    seq_t cur = (seq_t)row;
    for (idx_t i = 0; i <= pos; ++i)
        cur = next_value(cur);

    return cur;
}
