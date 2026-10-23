/*
 * A107479  a(n) = a(n-2) + a(n-3) + a(n-4) + a(n-5) + a(n-6) + a(n-7).
 * https://oeis.org/A107479
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 25, u32 n <= 49, u64 n <= 97, u128 n <= 192,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-2)+a(n-3)+a(n-4)+a(n-5)+a(n-6)+a(n-7)
   with a(0)=0, a(1)=1, a(2)=1, a(3)=2, a(4)=3, a(5)=5, a(6)=8, a(7)=12 */
#include <stdlib.h>

static seq_t *dp = NULL;
static idx_t dp_size = 0;

seq_t a(idx_t n)
{
    /* Ensure the table has space for indices up to n */
    if (dp_size == 0) {
        dp = (seq_t*)malloc(8 * sizeof(seq_t));
        if (!dp) return 0;      /* allocation failed – return 0 as fallback */
        dp_size = 8;
        dp[0] = 0; dp[1] = 1; dp[2] = 1; dp[3] = 2;
        dp[4] = 3; dp[5] = 5; dp[6] = 8; dp[7] = 12;
    }
    if ((idx_t)n >= dp_size) {
        size_t new_sz = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(dp, new_sz * sizeof(seq_t));
        if (!tmp) return 0;      /* allocation failed – return 0 */
        dp = tmp;
        for (idx_t i = dp_size; i <= n; ++i) {
            if (i <= 7) continue;   /* base values already set */
            dp[i] = dp[i-2] + dp[i-3] + dp[i-4] + dp[i-5] + dp[i-6] + dp[i-7];
        }
        dp_size = new_sz;
    }
    return dp[n];
}
