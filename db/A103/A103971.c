/*
 * A103971  Expansion of (1 - sqrt(1 - 4*x - 16*x^2))/(2*x).
 * https://oeis.org/A103971
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 26, u128 n <= 49,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (2*(2*n-1)*a(n-1) + 16*(n-2)*a(n-2)) / (n+1)
 * Recurrence with a(0) = 1, a(1) = 5
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    /* static buffer holding already computed terms */
    static seq_t *dp = NULL;
    static idx_t dp_cap = 0;   /* allocated capacity (power of two) */
    static idx_t dp_len = 0;   /* number of terms actually stored (max index + 1) */

    /* --- ensure storage is large enough --- */
    if ((idx_t)n >= dp_cap) {
        idx_t newcap = dp_cap ? (dp_cap << 1) : 4;
        while (newcap <= (idx_t)n) newcap <<= 1;
        seq_t *newdp = alloc_seq_array(newcap);
        for (idx_t i = 0; i < dp_cap; ++i) newdp[i] = dp[i];
        if (dp) free_seq_array(dp);
        dp = newdp;
        dp_cap = newcap;
    }

    /* --- initial values --- */
    if (dp_len == 0) {
        dp[0] = (seq_t)1;
        dp[1] = (seq_t)5;
        dp_len = 2;
    }

    /* --- compute any missing terms up to n --- */
    for (idx_t i = dp_len; i <= (idx_t)n; ++i) {
        if (i < 2) {           /* already set */
            continue;
        }

#if defined(__cplusplus)
        /* C++ build: seq_t may be a big‑integer class.
           Operations are performed with seq_t itself; no overflow. */
        seq_t term1 = dp[i-1] * (seq_t)(2 * (2 * i - 1));
        seq_t term2 = dp[i-2] * (seq_t)(16 * (i - 2));
        seq_t num   = term1 + term2;
        dp[i] = num / (seq_t)(i + 1);
#else
        /* C build: seq_t is an unsigned integer type (8–128 bits).
           Use an unsigned __int128 temporary to avoid overflow. */
        unsigned __int128 mul1 = (unsigned __int128)dp[i-1] * (unsigned __int128)(2 * (2 * i - 1));
        unsigned __int128 mul2 = (unsigned __int128)dp[i-2] * (unsigned __int128)(16 * (i - 2));
        unsigned __int128 num  = mul1 + mul2;
        dp[i] = (seq_t)(num / (unsigned __int128)(i + 1));
#endif
    }

    dp_len = (idx_t)n + 1;
    return dp[(idx_t)n];
}
