/*
 * A103972  Expansion of (1-sqrt(1-4*x-20*x^2))/(2*x).
 * https://oeis.org/A103972
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: (n+1)*a(n)=2*(2n-1)*a(n-1)+20*(n-2)*a(n-2);
   a(0)=1, a(1)=6 */

#include <stdlib.h>
#include <stdint.h>

#if defined(__SIZEOF_INT128__)
#define HAVE_INT128 1
#else
#define HAVE_INT128 0
#endif

seq_t a(idx_t n)
{
    static seq_t *dp = NULL;
    static idx_t max_n = 0;

    /* initial cache */
    if (dp == NULL) {
        dp = (seq_t *)calloc(2, sizeof(seq_t));
        dp[0] = (seq_t)1;
        dp[1] = (seq_t)6;
        max_n   = 1;
    }

    if (n <= max_n)
        return dp[n];

    dp = (seq_t *)realloc(dp, (n + 1) * sizeof(seq_t));

    for (idx_t i = max_n + 1; i <= n; ++i) {
        if (HAVE_INT128 && sizeof(seq_t) <= 8) {
            /* Use a 128‑bit unsigned type to avoid overflow for integral types */
            unsigned __int128 num =
                (unsigned __int128)2 * (unsigned __int128)(2 * i - 1) * (unsigned __int128)dp[i - 1] +
                (unsigned __int128)20 * (unsigned __int128)(i - 2) * (unsigned __int128)dp[i - 2];
            dp[i] = (seq_t)(num / (i + 1));
        } else {
            /* For arbitrary‑precision types, rely on the provided operators */
            seq_t term1 = dp[i - 1] * (seq_t)(2 * (2 * i - 1));
            seq_t term2 = dp[i - 2] * (seq_t)(20 * (i - 2));
            dp[i] = (term1 + term2) / (seq_t)(i + 1);
        }
    }

    max_n = n;
    return dp[n];
}
