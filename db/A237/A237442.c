/*
 * A237442  a(n) is the least number of 3-smooth numbers that add up to n.
 * https://oeis.org/A237442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = least number of 3‑smooth numbers (powers of 2 or 3) whose
 *            sum is n (i.e. the classic 3‑smooth coin change problem). The
 *            implementation uses a bottom‑up dynamic programming algorithm.
 *            For every n it keeps a memoised DP array `minv` that stores the
 *            minimal number of 3‑smooth addends for all...
 * Range:     u8 n <= 1033, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gen_3smooth
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Standard headers needed for malloc, free, size_t */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/gen_3smooth.h"

/*  The build supplies seq_t and idx_t types.  No other headers are needed. */

seq_t a(idx_t n)
{
    /*  DP memoised between calls */
    static seq_t *minv = NULL;   /* minimal number of 3‑smooth addends */
    static size_t caps = 0;      /* size of allocated array (>= n+1) */

    if ((size_t)n + 1 > caps) { /*  need to extend DP up to `n` */
        size_t new_cap = (size_t)n + 1;

        /*  Create list of 3‑smooth numbers up to n */
        size_t coins_cap = new_cap * 2;   /* plenty of space */
        seq_t *coins = (seq_t *)malloc(coins_cap * sizeof(seq_t));
        if (!coins)
            return 0; /* allocation failure – unlikely under test harness */
        size_t coin_len = 0;
        gen_3smooth(coins, &coin_len, coins_cap, n);

        /*  Allocate/resize DP array */
        seq_t *new_min = (seq_t *)calloc(new_cap, sizeof(seq_t));
        if (!new_min) {
            free(coins);
            return 0;
        }
        seq_t max_val = (seq_t)new_cap;          /* at most `n` ones */
        for (size_t i = 0; i < new_cap; ++i)
            new_min[i] = max_val;
        new_min[0] = 0;                          /* 0 needs 0 addends */

        /*  Relax DP with each 3‑smooth coin */
        for (size_t idx = 0; idx < coin_len; ++idx) {
            size_t coin = (size_t)coins[idx];
            for (size_t i = coin; i < new_cap; ++i) {
                seq_t cand = new_min[i - coin] + 1;
                if (cand < new_min[i])
                    new_min[i] = cand;
            }
        }

        /*  Replace old memoised array */
        free(minv);
        minv = new_min;
        caps = new_cap;

        free(coins);
    }

    return minv[(size_t)n];
}
