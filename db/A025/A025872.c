/*
 * A025872  Expansion of 1/((1-x^4)*(1-x^8)*(1-x^9)).
 * https://oeis.org/A025872
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of non‑negative integer solutions to 4*x + 8*y +
 *            9*z = n
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>            /* malloc, free, calloc */

seq_t a(idx_t n)
{
    /* Two allocation methods: use C++ new when compiling as C++,
       otherwise use calloc to produce zeroed memory. */
#ifdef __cplusplus
    seq_t *dp = new seq_t[n + 1]();   /* default-constructed elements */
#else
    seq_t *dp = (seq_t *)calloc(n + 1, sizeof(seq_t));
#endif
    if (!dp)
        return (seq_t)0;   /* memory allocation failed */

    dp[0] = (seq_t)1;   /* one way to form 0: use no parts */

    const idx_t parts[3] = {4, 8, 9};
    for (size_t pi = 0; pi < 3; ++pi) {
        idx_t part = parts[pi];
        for (idx_t i = part; i <= n; ++i) {
            dp[i] += dp[i - part];
        }
    }

    seq_t result = dp[n];

#ifdef __cplusplus
    delete[] dp;
#else
    free(dp);
#endif
    return result;
}
