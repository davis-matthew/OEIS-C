/*
 * A107025  Binomial transform of the expansion of 1/(1-x^5-x^6).
 * https://oeis.org/A107025
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 6*a(n-1)-15*a(n-2)+20*a(n-3)-15*a(n-4)+7*a(n-5)-a(n-6)
 *            Initial values: a(0)=1,a(1)=1,a(2)=1,a(3)=1,a(4)=1,a(5)=2
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   mul_shift_add
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mul_shift_add.h"

seq_t a(idx_t n)
{
    static seq_t *dp  = NULL;
    static idx_t size = 0;
    static idx_t cap  = 0;

    /* initialise the first six values once */
    if (dp == NULL) {
        cap = 64;
        dp  = (seq_t *)malloc(cap * sizeof(seq_t));
        if (dp == NULL) return 0;            /* allocation failed */

        dp[0] = 1; dp[1] = 1; dp[2] = 1;
        dp[3] = 1; dp[4] = 1; dp[5] = 2;
        size = 6;
    }

    if (n < size) return dp[n];

    /* expand storage if required */
    if ((idx_t)(n + 1) > cap) {
        idx_t newcap = cap;
        while ((idx_t)(n + 1) > newcap) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(dp, newcap * sizeof(seq_t));
        if (tmp == NULL) return 0;            /* allocation failed */
        dp = tmp;
        cap = newcap;
    }

    /* compute remaining terms using the linear recurrence */
    for (idx_t i = size; i <= n; ++i) {
        if (i < 6) continue;          /* already initialised */

        seq_t pos = mul_shift_add(dp[i-1], 6);
        pos += mul_shift_add(dp[i-3], 20);
        pos += mul_shift_add(dp[i-5], 7);

        seq_t neg = mul_shift_add(dp[i-2], 15);
        neg += mul_shift_add(dp[i-4], 15);
        neg += dp[i-6];

        dp[i] = pos - neg;            /* result is guaranteed non‑negative */
    }

    size = n + 1;
    return dp[n];
}
