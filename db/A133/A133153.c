/*
 * A133153  Number of partitions of n into parts that are odd or == +- 2 (mod
 *          10).
 * https://oeis.org/A133153
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into parts that are odd * or
 *            congruent to ±2 (mod 10). Pseudo‑code: * dp[0] = 1 * for each
 *            allowed part p ≤ n * for i = p .. n * dp[i] += dp[i – p]
 * Range:     u8 n <= 19, u16 n <= 54, u32 n <= 167, u64 n <= 561,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static buffer reused for successive calls */
    static seq_t *dp = NULL;        /* dp[i] = number of partitions of i */
    static size_t dp_capacity = 0;  /* allocated size of dp */

    /* Allocate or enlarge dp array as needed */
    if ((size_t)n + 1 > dp_capacity) {
        seq_t *tmp = (seq_t *)realloc(dp, ((size_t)n + 1) * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failed */
        dp = tmp;
        dp_capacity = n + 1;
    }

    /* initialize dp[0] and zero the rest */
    dp[0] = 1;
    for (idx_t i = 1; i <= n; ++i) dp[i] = 0;

    /* dynamic programming over allowed parts */
    for (idx_t part = 1; part <= n; ++part) {
        bool allowed = false;
        if (part & 1U) {            /* odd parts */
            allowed = true;
        } else {                    /* even parts only if ≡ ±2 (mod 10) */
            if (part % 10 == 2 || part % 10 == 8)
                allowed = true;
        }
        if (!allowed)
            continue;

        for (idx_t i = part; i <= n; ++i) {
            dp[i] += dp[i - part];
        }
    }

    return dp[n];
}
