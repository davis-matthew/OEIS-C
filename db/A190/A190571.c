/*
 * A190571  Number of n-step one-sided prudent walks, avoiding exactly three
 *          consecutive west steps and three consecutive east steps.
 * https://oeis.org/A190571
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, walk
 * Formula:   a(n) = 2*a(n-1) + a(n-2) - 2*a(n-4) + 2*a(n-5) Initial values
 *            (offset 1): a(1)=1, a(2)=3, a(3)=7, a(4)=15, a(5)=37
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 26, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static memoization array; index 0 stores 0 for convenience */
    static seq_t *memo = NULL;
    static size_t sz = 0;   /* number of initialized elements (may be >0) */
    static size_t cap = 0;  /* allocated capacity */

    /* initialise on first call */
    if (!memo) {
        cap = 64;
        memo = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!memo) abort();      /* out‑of‑memory */
        memo[0] = 0;
        memo[1] = 1;
        memo[2] = 3;
        memo[3] = 7;
        memo[4] = 15;
        memo[5] = 37;
        sz = 6;                    /* elements 0..5 are defined */
    }

    /* extend storage if necessary */
    if (n + 1 > cap) {
        size_t new_cap = (cap > 0) ? cap : 64;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        memo = tmp;
        cap = new_cap;
    }

    /* compute new terms up to n if required */
    if (n >= sz) {
        for (idx_t i = sz; i <= n; ++i) {
            seq_t val = memo[i - 1] * 2 + memo[i - 2];
            val = val - memo[i - 4] * 2 + memo[i - 5] * 2;
            memo[i] = val;
        }
        sz = n + 1;
    }

    return memo[n];
}
