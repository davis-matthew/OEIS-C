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
 * Validated: standard, pass, 103 terms through n = 9834 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Standard headers needed for malloc, free, size_t */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/*  Generate all 3‑smooth numbers <= limit into `coins`.  `cap` is the array
    capacity; the function guarantees that no more than `cap` numbers are
    written.  It is safe to pass a sufficiently large capacity (here we use
    2*max_n+10). */
static void gen_3smooth(seq_t *coins, size_t *len, size_t cap, idx_t limit)
{
    *len = 0;
    for (idx_t a = 0; ; ++a) {
        seq_t base = 1;                   /* 2^a */
        for (idx_t i = 0; i < a; ++i)
            base *= 2;
        if (base > limit)
            break;

        for (idx_t b = 0; ; ++b) {
            seq_t val = base;             /* 2^a 3^b */
            for (idx_t j = 0; j < b; ++j)
                val *= 3;
            if (val > limit)
                break;

            if (*len == cap)   /* should not happen with large enough cap */
                return;
            coins[(*len)++] = val;
        }
    }
}

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
        seq_t *coins = mem_new(seq_t, coins_cap);
        if (!coins)
            return 0; /* allocation failure – unlikely under test harness */
        size_t coin_len = 0;
        gen_3smooth(coins, &coin_len, coins_cap, n);

        /*  Allocate/resize DP array */
        seq_t *new_min = mem_new(seq_t, new_cap);
        if (!new_min) {
            mem_free(coins);
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
        mem_free(minv);
        minv = new_min;
        caps = new_cap;

        mem_free(coins);
    }

    return minv[(size_t)n];
}
