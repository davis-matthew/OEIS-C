/*
 * A107856  a(n) = 2*a(n-2)+4*a(n-4)+a(n-6), n>9.
 * https://oeis.org/A107856
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-2)+4*a(n-4)+a(n-6), n > 9 Initial values:
 *            a(1)=1,a(2)=1,a(3)=1,a(4)=0,a(5)=4,a(6)=4,
 *            a(7)=11,a(8)=7,a(9)=37,a(10)=30
 * Range:     u8 n <= 12, u16 n <= 22, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *dp = NULL;      /* dynamic table of computed values */
static idx_t dp_size = 0;     /* current capacity (dp_size > n implied) */

static const seq_t init[11] = {0,1,1,1,0,4,4,11,7,37,30};

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset starts at 1; 0 is undefined */

    /* initial allocation and base values */
    if (!dp) {
        dp_size = 11;                /* indices 0..10 (0 unused) */
        dp = (seq_t*)malloc(dp_size * sizeof(seq_t));
        if (!dp) return 0;          /* memory error → return 0 */
        for (idx_t i = 1; i < dp_size; ++i) dp[i] = init[i];
    }

    /* extend table if necessary */
    if (n >= dp_size) {
        seq_t *newdp = (seq_t*)realloc(dp, (n + 1) * sizeof(seq_t));
        if (!newdp) return 0;        /* memory error → return 0 */
        dp = newdp;
        for (idx_t i = dp_size; i <= n; ++i) {
            if (i <= 10) {
                dp[i] = init[i];
            } else {
                dp[i] = ((seq_t)2) * dp[i-2]  /* 2*a(n-2) */
                     + ((seq_t)4) * dp[i-4]  /* 4*a(n-4) */
                     +       dp[i-6];      /* + a(n-6) */
            }
        }
        dp_size = n + 1;
    }

    return dp[n];
}
