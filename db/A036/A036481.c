/*
 * A036481  a(n) = partition(11n+8) mod 11.
 * https://oeis.org/A036481
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = partition(11*n + 8) mod 11
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* target value */
    idx_t m = (idx_t)11 * n + (idx_t)8;

    /* static cache: dp[i] = partition(i) mod 11 */
    static uint8_t *dp   = NULL;      /* mod‑11 values */
    static size_t   dp_sz = 0;        /* number of stored entries */

    /* the cache must hold indices up to m */
    size_t new_sz = (size_t)m + 1;
    size_t old_sz = dp_sz;

    if (new_sz > dp_sz) {
        /* enlarge the buffer */
        uint8_t *tmp = (uint8_t *)realloc(dp, new_sz);
        if (!tmp) return (seq_t)0;   /* allocation failed – return 0 */
        dp   = tmp;
        /* initialise new entries to 0 */
        for (size_t i = old_sz; i < new_sz; ++i)
            dp[i] = 0;
        /* initialise p(0) */
        if (old_sz == 0)
            dp[0] = 1;
        dp_sz = new_sz;
    }

    /* compute new partition values modulo 11, if any */
    for (size_t i = old_sz; i <= (size_t)m; ++i) {
        if (i == 0) {                 /* partition(0) = 1 */
            dp[0] = 1;
            continue;
        }
        int temp = 0;                 /* running sum modulo 11 */
        for (int k = 1; ; ++k) {
            idx_t G1 = (idx_t)k * ((idx_t)3 * k - 1) / 2;
            idx_t G2 = (idx_t)k * ((idx_t)3 * k + 1) / 2;

            if (G1 > i && G2 > i) break;

            int sign = (k & 1) ? 1 : -1;
            if (G1 <= i) {
                int add = dp[(size_t)i - (size_t)G1];
                if (sign > 0) {
                    temp += add;
                    if (temp >= 11) temp -= 11;
                } else {
                    temp -= add;
                    if (temp < 0)  temp += 11;
                }
            }
            if (G2 <= i) {
                int add = dp[(size_t)i - (size_t)G2];
                if (sign > 0) {
                    temp += add;
                    if (temp >= 11) temp -= 11;
                } else {
                    temp -= add;
                    if (temp < 0)  temp += 11;
                }
            }
        }
        dp[i] = (uint8_t)temp;
    }

    return (seq_t)dp[(size_t)m];
}
