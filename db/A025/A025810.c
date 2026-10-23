/*
 * A025810  Expansion of 1/((1-x^2)*(1-x^5)*(1-x^10)).
 * https://oeis.org/A025810
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of partitions of n into parts 2, 5 and 10.
 *
 * The function uses a simple dynamic‑programming
 * coin‑change algorithm.  It works for any integer
 * type seq_t that supports the usual arithmetic and
 * assignment operators, as supplied by the build.
 *
 * It supports both C and C++ builds: in the C++ build
 * a std::vector is used to avoid the hassle of manually
 * constructing objects with malloc; in the C build the
 * array is handled with malloc/realloc.
 *
 * The function is fully memoised: a static buffer is
 * reused and grown only when a larger n is requested.
 */

#include <stdlib.h>

#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
#ifdef __cplusplus
    /* C++: use a static vector for the DP table */
    static std::vector<seq_t> dp;
    /* grow the vector if needed */
    if (dp.size() < (size_t)(n + 1))
        dp.resize(n + 1);

    /* initialise */
    dp[0] = 1;
    for (idx_t i = 1; i <= n; ++i)
        dp[i] = 0;

    /* coin values */
    const int coins[3] = {2, 5, 10};
    for (int c = 0; c < 3; ++c) {
        idx_t coin = (idx_t)coins[c];
        for (idx_t i = coin; i <= n; ++i)
            dp[i] += dp[i - coin];
    }

    return dp[n];
#else
    /* C build: use a static malloc‑based array */
    static seq_t *dp = NULL;
    static idx_t dp_size = 0;

    /* grow the buffer if needed */
    if (dp == NULL || n + 1 > dp_size) {
        seq_t *tmp = (seq_t *)realloc(dp, (size_t)(n + 1) * sizeof(seq_t));
        if (!tmp)
            return 0;               /* allocation failure */
        dp = tmp;
        dp_size = n + 1;
    }

    /* initialise */
    dp[0] = 1;
    for (idx_t i = 1; i <= n; ++i)
        dp[i] = 0;

    /* coin values */
    const int coins[3] = {2, 5, 10};
    for (int c = 0; c < 3; ++c) {
        idx_t coin = (idx_t)coins[c];
        for (idx_t i = coin; i <= n; ++i)
            dp[i] += dp[i - coin];
    }

    return dp[n];
#endif
}
