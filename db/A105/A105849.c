/*
 * A105849  Row sums of number triangle A105848.
 * https://oeis.org/A105849
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) satisfies the linear recurrence (n+2)*a(n) +
 *            2*(-4*n-3)*a(n-1) + 4*(5*n-2)*a(n-2) + 2*(-10*n+17)*a(n-3) +
 *            8*(n-3)*a(n-4) = 0 with a(0)=1, a(1)=3, a(2)=9, a(3)=28
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Standard allocation helpers (provided by the build) are not used;
   instead we use malloc/realloc directly. */
static seq_t *dp = NULL;
static size_t dp_cap = 0;
static idx_t max_computed = 0;

seq_t a(idx_t n)
{
    /* initialise cache with the first four values */
    if (dp == NULL) {
        dp = (seq_t *)malloc(4 * sizeof(seq_t));
        if (!dp) return (seq_t)0;          /* allocation failure; will give garbage */
        dp[0] = (seq_t)1;
        dp[1] = (seq_t)3;
        dp[2] = (seq_t)9;
        dp[3] = (seq_t)28;
        dp_cap = 4;
        max_computed = 3;
    }

    if (n <= max_computed)
        return dp[n];

    /* enlarge the array if necessary */
    if (dp_cap <= n) {
        size_t new_cap = dp_cap ? dp_cap : 1;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(dp, new_cap * sizeof(seq_t));
        if (tmp) {
            dp = tmp;
            dp_cap = new_cap;
        } else {
            /* realloc failed; return zero */
            return (seq_t)0;
        }
    }

    /* compute terms from max_computed+1 up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t result;
#if defined(__cplusplus) && (__cplusplus >= 201703L)
        /* C++17 branch: use 128‑bit integers for small integral seq_t types */
        if constexpr (sizeof(seq_t) <= 8 && std::is_integral_v<seq_t>) {
            using u128 = unsigned __int128;
            u128 t1 = (u128)(8 * (u128)i + 6) * (u128)dp[i-1];
            u128 t2 = (u128)(20 * (u128)i - 8) * (u128)dp[i-2];
            u128 t3 = (u128)(20 * (u128)i - 34) * (u128)dp[i-3];
            u128 t4 = (u128)(8 * (u128)i - 24) * (u128)dp[i-4];
            u128 numer = t1 - t2 + t3 - t4;
            result = (seq_t)(numer / ((u128)i + 2));
        } else
#endif
        {
            /* generic arithmetic that works for arbitrary‑precision types */
            seq_t t1 = ((seq_t)8 * (seq_t)i + (seq_t)6) * dp[i-1];
            seq_t t2 = ((seq_t)20 * (seq_t)i - (seq_t)8) * dp[i-2];
            seq_t t3 = ((seq_t)20 * (seq_t)i - (seq_t)34) * dp[i-3];
            seq_t t4 = ((seq_t)8 * (seq_t)i - (seq_t)24) * dp[i-4];
            seq_t numer = t1 - t2 + t3 - t4;
            result = numer / ((seq_t)i + (seq_t)2);
        }
        dp[i] = result;
    }

    max_computed = n;
    return dp[n];
}
