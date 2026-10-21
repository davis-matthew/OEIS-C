/*
 * A238007  Number of strict partitions of n such that (greatest part) -
 *          (least part) >= (number of parts).
 * https://oeis.org/A238007
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000009(n) - A001227(n) A000009(n) = number of partitions
 *            of n into distinct parts A001227(n) = number of partitions of n
 *            into consecutive positive integers (including the single‑part
 *            case)
 * Range:     u8 n <= 29, u16 n <= 78, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* ---------- compute A000009(n) : partitions into distinct parts ---------- */
    static seq_t *dp   = NULL;   /* dp[i] = number of strict partitions of i */
    static idx_t dpmax = 0;      /* largest n for which dp[0..dpmax] were filled */

    if (dp == NULL || n > dpmax) {
        /* allocate or extend the table */
        dp = (seq_t *)realloc(dp, (size_t)(n + 1) * sizeof(seq_t));
        if (dp == NULL) return (seq_t)0;   /* allocation failure */

        /* reset and recompute from scratch for the new maximum */
        for (idx_t i = 0; i <= n; ++i) dp[i] = (seq_t)0;
        dp[0] = (seq_t)1;                    /* empty partition */

        for (idx_t part = 1; part <= n; ++part) {
            for (idx_t i = n; i >= part; --i) {
                dp[i] = dp[i] + dp[i - part];
            }
        }
        dpmax = n;
    }

    seq_t distinct = dp[n];

    /* ---------- compute A001227(n) : partitions into consecutive parts ---------- */
    seq_t consecutive = (seq_t)0;
    for (idx_t k = 1; k * (k + 1) / 2 <= n; ++k) {
        /* sum of k consecutive integers starting at a is k*a + k*(k-1)/2 = n   */
        idx_t numer = n - k * (k - 1) / 2;
        if (numer % k == 0 && numer / k >= 1)
            ++consecutive;
    }

    return distinct - consecutive;
}
