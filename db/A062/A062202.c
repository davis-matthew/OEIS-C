/*
 * A062202  Number of compositions of n such that two adjacent parts are not
 *          equal modulo 4.
 * https://oeis.org/A062202
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A062202: Number of compositions of n such that two adjacent parts
 * are not equal modulo 4.
 * Recurrence:
 *   Let dp[n][m] be the number of such compositions of n whose last part
 *   is congruent to m modulo 4 (m = 0,1,2,3).  Then for n ≥ 1
 *     dp[n][m] = 1 (for the single-part composition n, if n ≡ m (mod 4))
 *                + Σ_{k=1}^{n-1, k≡m (mod 4)} Σ_{p≠m} dp[n−k][p]
 *   and a(n) = Σ_{m=0}^3 dp[n][m].
 * Initial values: a(0)=1, a(1)=1, a(2)=1, a(3)=3, a(4)=4.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

static seq_t *dp = NULL;          /* flattened array dp[n][m] → dp[n*4 + m] */
static size_t dp_cap = 0;         /* number of n values currently stored (0..dp_cap-1) */

/* Ensure dp contains values for all indices 1 .. n (a(0)=1 is handled separately). */
static void ensure_cache(idx_t n)
{
    if (n == 0) return;
    size_t needed = (size_t)n + 1;      /* we want entries for 0 .. n inclusive */
    if (needed <= dp_cap) return;       /* already computed */

    seq_t *tmp = (seq_t*)realloc(dp, needed * 4 * sizeof(seq_t));
    if (!tmp) exit(1);
    dp = tmp;

    /* Compute new entries from dp_cap up to needed-1 */
    for (size_t i = dp_cap; i < needed; ++i) {
        size_t base = i * 4;
        dp[base]     = dp[base + 1] = dp[base + 2] = dp[base + 3] = (seq_t)0;

        for (size_t k = 1; k <= i; ++k) {
            unsigned m = (unsigned)(k & 3);          /* k % 4 */
            size_t idx = base + m;
            if (k == i) {
                /* Single‑part composition */
                dp[idx] += (seq_t)1;
            } else {
                size_t prev_base = (i - k) * 4;
                /* Sum of dp[i-k][p] for all p ≠ m */
                seq_t sum = dp[prev_base + ((m + 1) & 3)] +
                            dp[prev_base + ((m + 2) & 3)] +
                            dp[prev_base + ((m + 3) & 3)];
                dp[idx] += sum;
            }
        }
    }
    dp_cap = needed;
}

/* Public function required by the testing harness. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;   /* base case: empty composition */
    ensure_cache(n);
    size_t base = (size_t)n * 4;
    seq_t result = dp[base];
    result += dp[base + 1];
    result += dp[base + 2];
    result += dp[base + 3];
    return result;
}
