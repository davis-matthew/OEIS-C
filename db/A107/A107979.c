/*
 * A107979  a(n) = 4*a(n-1) + 2*a(n-2) for n>1, with a(0)=2, a(1)=9.
 * https://oeis.org/A107979
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) + 2*a(n-2), a(0)=2, a(1)=9
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *dp = NULL;
static size_t dp_size = 0;
static size_t dp_cap   = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 2;
    if (n == 1) return 9;

    if (dp == NULL) {
        dp_cap = 4;
        dp = (seq_t*)malloc(dp_cap * sizeof(seq_t));
        dp[0] = 2;
        dp[1] = 9;
        dp_size = 2;
    }

    if (dp_size <= n) {
        if (dp_cap <= n) {
            size_t new_cap = dp_cap;
            while (new_cap <= n) new_cap <<= 1;
            seq_t *tmp = (seq_t*)realloc(dp, new_cap * sizeof(seq_t));
            dp = tmp;
            dp_cap = new_cap;
        }
        for (size_t i = dp_size; i <= n; ++i) {
            dp[i] = 4 * dp[i-1] + 2 * dp[i-2];
        }
        dp_size = n + 1;
    }

    return dp[n];
}
