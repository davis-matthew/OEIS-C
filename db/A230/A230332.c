/*
 * A230332  Number of nX3 0..2 arrays x(i,j) with each element horizontally,
 *          vertically or antidiagonally next to at least one element with
 *          value (x(i,j)+1) mod 3, and upper left element zero.
 * https://oeis.org/A230332
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 15*a(n-1)+37*a(n-2)+6*a(n-3)-137*a(n-4)-75*a(n-5) *
 *            +577*a(n-6)+1558*a(n-7)+1020*a(n-8)-8*a(n-9) for n>10 * Initial
 *            terms: a(1)=0, a(2)=34, a(3)=534, a(4)=9162, a(5)=157200, *
 *            a(6)=2696012, a(7)=46237080, a(8)=792976550, * a(9)=13599731534,
 *            a(10)=233238537338
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initial values for n = 1..10, index 0 unused */
    static const seq_t init[11] = {
        0ULL,            /* dummy for index 0 */
        0ULL,            /* a(1) */
        34ULL,           /* a(2) */
        534ULL,          /* a(3) */
        9162ULL,         /* a(4) */
        157200ULL,       /* a(5) */
        2696012ULL,      /* a(6) */
        46237080ULL,     /* a(7) */
        792976550ULL,    /* a(8) */
        13599731534ULL,  /* a(9) */
        233238537338ULL  /* a(10) */
    };

    if (n <= 10) return init[(size_t)n];

    /* Memoization array, 1‑based indexing */
    static seq_t *dp = NULL;
    static size_t dp_cap = 0;
    static idx_t last_computed = 0;

    /* Allocate or grow dp array to hold up to n */
    if (dp == NULL) {
        dp_cap = (size_t)n + 1;
        dp = (seq_t *)malloc(dp_cap * sizeof(seq_t));
        for (size_t i = 1; i <= 10; ++i) dp[i] = init[i];
        last_computed = 10;
    } else if ((size_t)n + 1 > dp_cap) {
        dp_cap = (size_t)n + 1;
        dp = (seq_t *)realloc(dp, dp_cap * sizeof(seq_t));
    }

    /* Compute new terms if needed */
    if (n > last_computed) {
        for (size_t i = (size_t)last_computed + 1; i <= (size_t)n; ++i) {
            /* Positive contributions */
            seq_t pos = 0;
            pos += dp[i - 1] * 15;
            pos += dp[i - 2] * 37;
            pos += dp[i - 3] * 6;
            pos += dp[i - 6] * 577;
            pos += dp[i - 7] * 1558;
            pos += dp[i - 8] * 1020;

            /* Negative contributions */
            seq_t neg = 0;
            neg += dp[i - 4] * 137;
            neg += dp[i - 5] * 75;
            neg += dp[i - 9] * 8;

            /* The recurrence guarantees pos >= neg */
            dp[i] = pos - neg;
        }
        last_computed = n;
    }

    return dp[(size_t)n];
}
