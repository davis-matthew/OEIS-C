/*
 * A161954  Number of reduced words of length n in the Weyl group B_27.
 * https://oeis.org/A161954
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = coefficient of x^n in ∏_{k=1}^{27} (1-x^(2k))/(1-x)
 *            (Number of reduced words of length n in the Weyl group B_27)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 12, u64 n <= 46, u128 n <= 729,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/seq_from_str.h"

seq_t a(idx_t n)
{
    const size_t M    = 27;           /* rank of B_27   */
    const size_t MAXN = M * M;        /* 729, maximum possible n with non‑zero term */

    static __int128 *dp  = NULL;      /* DP array holding intermediate coefficients   */
    static __int128 *tmp = NULL;      /* temporary array used in the convolution step */

    /* initialise the DP tables on first call */
    if (!dp) {
        dp = (__int128 *)malloc((MAXN + 1) * sizeof(__int128));
        if (!dp) exit(EXIT_FAILURE);
        for (size_t i = 0; i <= MAXN; ++i) dp[i] = 0;
        dp[0] = 1;                   /* empty product */

        tmp = (__int128 *)malloc((MAXN + 1) * sizeof(__int128));
        if (!tmp) exit(EXIT_FAILURE);

        for (size_t k = 1; k <= M; ++k) {
            size_t len = 2 * k;                         /* powers 0..2k-1 */
            for (size_t i = 0; i <= MAXN; ++i) tmp[i] = 0;

            for (size_t s = 0; s <= MAXN; ++s)
                if (dp[s]) {
                    __int128 val = dp[s];
                    for (size_t a = 0; a < len && (s + a) <= MAXN; ++a)
                        tmp[s + a] += val;
                }
            memcpy(dp, tmp, (MAXN + 1) * sizeof(__int128));
        }
    }

    if (n > MAXN) return seq_from_str("0");

    __int128 v = dp[n];
    if (v == 0) return seq_from_str("0");

    /* convert the 128‑bit integer to a decimal string so we can call seq_from_str   */
    char buf[50];
    size_t pos = 0;
    unsigned __int128 x = (v < 0) ? (unsigned __int128)(-v) : (unsigned __int128)v;

    while (x > 0) {
        unsigned digit = (unsigned)(x % 10);
        buf[pos++] = '0' + digit;
        x /= 10;
    }
    if (v < 0) buf[pos++] = '-';

    /* reverse into correct order */
    for (size_t i = 0; i < pos / 2; ++i) {
        char t   = buf[i];
        buf[i]   = buf[pos - 1 - i];
        buf[pos - 1 - i] = t;
    }
    buf[pos] = '\0';

    return seq_from_str(buf);
}
