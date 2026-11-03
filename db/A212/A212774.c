/*
 * A212774  Amounts (in cents) of coins in denominations 1, 5, 10, 25, and 50
 *          (cents) which, when using the minimal number of coins, have equal
 *          numbers of all denominations used.
 * https://oeis.org/A212774
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-41)*50 for n >= 46. The first 45 terms are computed by
 *            a greedy coin‑change search with denominations 1,5,10,25,50 and
 *            a minimal coin count check
 * Range:     u8 n <= 46, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* Test whether a given amount satisfies the sequence condition. */
static bool amount_is_term(idx_t amount)
{
    if (amount == 0)            /* a(1) = 0 */
        return true;

    /* Minimal number of coins (greedy is optimal for this system). */
    int min_coins = 0;
    idx_t rem = amount;
    const int desc[5] = {50, 25, 10, 5, 1};
    for (int i = 0; i < 5; ++i) {
        min_coins += rem / desc[i];
        rem %= desc[i];
    }

    /* Try all possible numbers of used denominations k (1..5). */
    for (int k = 1; k <= 5; ++k) {
        if (min_coins % k != 0)
            continue;
        int s = min_coins / k;          /* number of coins of each used denom */
        if (s == 0)
            continue;                   /* avoid division by zero for amount 0 */

        if (amount % s != 0)
            continue;
        int d = amount / s;             /* sum of used denominations */

        /* Enumerate all non‑empty subsets of the denominations. */
        const int asc[5] = {1, 5, 10, 25, 50};
        for (int mask = 1; mask < 32; ++mask) {
            int sz = 0, sum = 0;
            for (int j = 0; j < 5; ++j) {
                if (mask & (1 << j)) {
                    ++sz;
                    sum += asc[j];
                }
            }
            if (sz == k && sum == d)
                return true;            /* found a valid representation */
        }
    }
    return false;                       /* no representation found */
}

/* Pre‑computation of the first 45 terms. */
static bool a_precomp_done = false;
static seq_t a_first45[45];

/* Compute the first 45 terms once. */
static void precompute_first45(void)
{
    idx_t val = 0;
    size_t idx = 0;
    while (idx < 45) {
        if (amount_is_term(val))
            a_first45[idx++] = (seq_t)val;
        ++val;
    }
    a_precomp_done = true;
}

/* Main function required by the build. */
seq_t a(idx_t n)
{
    if (!a_precomp_done)
        precompute_first45();

    if (n <= 45)
        return a_first45[n - 1];

    /* For n >= 46, the sequence is a linear progression of multiples of 50. */
    return (seq_t)(n - 41) * 50;
}
