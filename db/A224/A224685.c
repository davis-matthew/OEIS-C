/*
 * A224685  Number of (n+4) X 7 0..1 matrices with each 5 X 5 subblock
 *          idempotent.
 * https://oeis.org/A224685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-3) - a(n-4) * - a(n-7) + a(n-8) -
 *            a(n-9) + a(n-10) * + a(n-12) - a(n-13) (n > 17) * initial terms:
 *            a(1)=2662, a(2)=1494, a(3)=1636, a(4)=1896, * a(5)=2136,
 *            a(6)=2354, a(7)=2862, a(8)=3701, a(9)=4814, * a(10)=6134,
 *            a(11)=7579, a(12)=9522, a(13)=12249, * a(14)=16025,...
 * Range:     u8 none, u16 n <= 19, u32 n <= 58, u64 n <= 137, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static const seq_t init[17] = {
    (seq_t)2662,(seq_t)1494,(seq_t)1636,(seq_t)1896,(seq_t)2136,
    (seq_t)2354,(seq_t)2862,(seq_t)3701,(seq_t)4814,(seq_t)6134,
    (seq_t)7579,(seq_t)9522,(seq_t)12249,(seq_t)16025,(seq_t)21050,
    (seq_t)27502,(seq_t)35816
};

static seq_t *dp = NULL;
static size_t dp_cap = 0;
static size_t dp_sz  = 0;

/* Ensure dp has room for at least 'need' elements. */
static void ensure_capacity(size_t need)
{
    if (dp_cap >= need) return;
    size_t new_cap = dp_cap ? dp_cap << 1 : 32;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(dp, new_cap * sizeof(seq_t));
    if (!tmp) abort();          /* allocation failure */
    dp = tmp;
    dp_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;   /* safety for n=0 */

    /* Ensure the first 17 terms are available. */
    if (dp_sz < 17) {
        ensure_capacity(17);
        for (size_t i = 0; i < 17; ++i) dp[i] = init[i];
        dp_sz = 17;
    }

    /* If the requested term is already cached, return it. */
    if ((size_t)n <= dp_sz) return dp[(size_t)n - 1];

    /* Compute terms up to n (inclusive). */
    size_t limit = (size_t)n;
    ensure_capacity(limit);
    for (size_t i = dp_sz + 1; i <= limit; ++i) {
        seq_t val;
        if (i <= 17) {
            val = init[i - 1];
        } else {
            /* Use the recurrence. Indices are 1‑based for a(i). */
            val = dp[i - 2];               /* dp[i-2] = a(i-1) */
            val += dp[i - 2];               /* 2*a(i-1) */
            val -= dp[i - 3];               /* - a(i-2) */
            val += dp[i - 4];               /* + a(i-3) */
            val -= dp[i - 5];               /* - a(i-4) */
            val -= dp[i - 8];               /* - a(i-7) */
            val += dp[i - 9];               /* + a(i-8) */
            val -= dp[i -10];               /* - a(i-9) */
            val += dp[i -11];               /* + a(i-10) */
            val += dp[i -13];               /* + a(i-12) */
            val -= dp[i -14];               /* - a(i-13) */
        }
        dp[i - 1] = val;
    }
    dp_sz = limit;
    return dp[(size_t)n - 1];
}
