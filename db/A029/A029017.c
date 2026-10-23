/*
 * A029017  Expansion of 1/((1-x)*(1-x^2)*(1-x^6)*(1-x^7)).
 * https://oeis.org/A029017
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 312, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1, and for n ≥ 1
   a(n) = a(n‑1) + a(n‑2) – a(n‑3) + a(n‑6) – 2·a(n‑8)
          + a(n‑10) – a(n‑13) + a(n‑14) + a(n‑15) – a(n‑16)
   (a(k) = 0 for k < 0).   This is the partition count with parts
   1, 2, 6 and 7, i.e. the OEIS sequence A029017. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for dynamic programming */
    static seq_t *dp   = NULL;   /* dp[i] = a(i) for i ≤ current max */
    static idx_t max_n = 0;      /* largest n whose value is cached */

    /* first call: create the array and initialise base case */
    if (dp == NULL) {
        dp = (seq_t *)malloc(sizeof(seq_t));
        if (!dp) return (seq_t)0;  /* allocation failed */
        dp[0] = (seq_t)1;          /* a(0) = 1 */
        max_n = 0;
    }

    /* if n is already cached, return it */
    if (n <= max_n) return dp[n];

    /* enlarge the cache to contain up to n */
    size_t new_size = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(dp, new_size * sizeof(seq_t));
    if (!tmp) return (seq_t)0;  /* reallocation failed */
    dp = tmp;

    /* compute new entries using the recurrence */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        seq_t sum = 0;
        if (i >= 1) sum += dp[i - 1];
        if (i >= 2) sum += dp[i - 2];
        if (i >= 3) sum -= dp[i - 3];
        if (i >= 6) sum += dp[i - 6];
        if (i >= 8) sum -= dp[i - 8] * 2;
        if (i >= 10) sum += dp[i - 10];
        if (i >= 13) sum -= dp[i - 13];
        if (i >= 14) sum += dp[i - 14];
        if (i >= 15) sum += dp[i - 15];
        if (i >= 16) sum -= dp[i - 16];
        dp[i] = sum;
    }
    max_n = n;
    return dp[n];
}
