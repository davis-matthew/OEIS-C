/*
 * A116374  Number of partitions of n into parts with digital root = 4.
 * https://oeis.org/A116374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   this function counts partitions of n into parts with digital
 *            root 4. A digital‑root‑4 number is ≡ 4 (mod 9), so the allowed
 *            parts are 4, 13, 22, 31, ... The routine uses a classic
 *            coin‑change dynamic programming with memoisation that recomputes
 *            the table whenever a larger n is requested
 * Range:     u8 n <= 96, u16 n <= 96, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache of partition numbers */
    static seq_t *dp = NULL;      /* dp[i] = number of partitions of i */
    static idx_t cur_max = 0;     /* largest n for which dp[0..cur_max] are up to date */

    /* if a larger n is requested, extend the cache */
    if (n > cur_max) {
        /* grow the array to hold indices up to n */
        size_t new_size = (size_t)(n + 1) * sizeof(seq_t);
        dp = (seq_t *)realloc(dp, new_size);
        if (!dp) return 0;        /* allocation failed – return 0 as a safe value */

        /* initialise dp array for indices 0..n */
        for (idx_t i = 0; i <= n; ++i) dp[i] = 0;
        dp[0] = 1;                /* empty partition */

        /* dynamic‑programming: for each allowed part p (≡4 mod 9) add its
           contribution to all larger indices. */
        for (idx_t p = 4; p <= n; p += 9) {
            for (idx_t i = p; i <= n; ++i) {
                dp[i] += dp[i - p];
            }
        }
        cur_max = n;               /* cache now valid up to n */
    }
    /* for n <= cur_max the table is already valid */
    return dp[n];
}
