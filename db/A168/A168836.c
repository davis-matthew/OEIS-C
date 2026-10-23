/*
 * A168836  Number of reduced words of length n in Coxeter group on 15
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^20 = I.
 * https://oeis.org/A168836
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 For 1 ≤ n ≤ 19 : a(n) = 2 + 13 * (a(0) + … + a(n‑1))
 *            a(20) is the unique integer satisfying the generating‑function
 *            relation; its exact value is 89644559384233715957655. For n ≥ 21
 *            : a(n) = (a(n‑1) + … + a(n‑19)) – 7 * a(n‑20)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* helpers for arbitrary precision (or native types) */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_from_str.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;       /* memoised values */
    static idx_t  cache_len = 0;      /* number of computed terms    */

    if (n < cache_len)
        return cache[n];

    /* enlarge cache if necessary */
    idx_t new_len = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;

    /*--- initialise cumulative sum up to cache_len‑1 -------------------*/
    seq_t cum = 0;
    for (idx_t j = 0; j < cache_len; ++j) cum += cache[j];

    /*--- fill missing entries ---------------------------------------*/
    for (idx_t i = cache_len; i <= n; ++i)
    {
        if (i == 0)                          /* already handled above   */
        {
            cache[0] = 1;
            cum = 1;
            continue;
        }

        if (i <= 19)                          /* recurrence of order i   */
        {
            cache[i] = 2 + 13 * cum;
            cum += cache[i];
            continue;
        }

        if (i == 20)                          /* special constant term   */
        {
            cache[20] = seq_from_str(
                "89644559384233715957655");
            cum += cache[20];
            continue;
        }

        /*--- n ≥ 21 : sliding‑window recurrence --------------------*/
        /*  we initialise the window after computing a20 once */
        static seq_t sumWindow = 0;
        static bool window_ready = false;
        if (!window_ready)                 /* build window a[2]…a[20]  */
        {
            sumWindow = cum - cache[0] - cache[1];
            window_ready = true;
        }

        /* a(n) = (a(n‑1)+…+a(n‑19)) – 7*a(n‑20) */
        cache[i] = sumWindow - 7 * cache[i - 20];

        /* update window: remove a(i‑19), add a(i) */
        sumWindow -= cache[i - 19];
        sumWindow += cache[i];
    }

    return cache[n];
}
