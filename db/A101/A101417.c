/*
 * A101417  Number of partitions of n into parts without powers of 2.
 * https://oeis.org/A101417
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Number of partitions of n into parts that are NOT powers of 2.
 *            a(n) = coefficient of x^n in Product_{j>=1} (1 - x^(2^j)) /
 *            Product_{i>=2} (1 - x^i)
 * Range:     u8 n <= 34, u16 n <= 77, u32 n <= 199, u64 n <= 578,
 *            u128 n <= 1843, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

seq_t a(idx_t n)
{
    /* a(0) = 1 by definition */
    if (n == 0)
        return (seq_t)1;

    /* Allocate an array of seq_t of length n+1          */
    /* Use malloc so the code compiles as C11 as well.    */
    size_t len = (size_t)(n + 1);
    seq_t *dp = (seq_t *)malloc(len * sizeof(seq_t));
    if (!dp)
        return (seq_t)0;           /* out‑of‑memory guard */

    /* Construct every element of the array.  For POD types
       the placement‑new is a no‑op; for non‑POD types it
       calls the default constructor.                     */
#ifdef __cplusplus
    for (size_t i = 0; i < len; ++i)
        new (&dp[i]) seq_t();
#endif

    /* Initialize DP values: dp[0] = 1, others 0             */
    dp[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        dp[i] = (seq_t)0;

    /* All allowed part sizes are integers >= 3 that are not
       powers of two.  Iterate over them and perform the classic
       coin‑change DP update.                               */
    for (idx_t part = 3; part <= n; ++part) {
        if ((part & (part - 1)) == 0)          /* skip 1,2,4,8,...   */
            continue;
        for (idx_t j = part; j <= n; ++j) {
            dp[j] += dp[j - part];
        }
    }

    /* Result is dp[n] */
    seq_t result = dp[n];

    /* Destruct every element before freeing memory.     */
#ifdef __cplusplus
    for (size_t i = 0; i < len; ++i)
        dp[i].~seq_t();
#endif

    free(dp);
    return result;
}
