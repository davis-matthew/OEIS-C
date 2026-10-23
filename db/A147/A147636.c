/*
 * A147636  a(n) = least n-distinct-decimal-digit number such that the string
 *          formed by the last k digits is divisible by k for any 1<=k<=n.
 * https://oeis.org/A147636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A147636:
 * a(n) = least n‑distinct‑decimal‑digit number such that for
 * every k (1≤k≤n) the last k digits are divisible by k.
 * If no such number exists (n>10) the function returns 0. */
#include <stdbool.h>

static void dfs(idx_t pos, idx_t n, uint32_t used, seq_t suffix,
                seq_t *best, bool *found, seq_t *pow10)
{
    if (pos == n) {
        if (!*found || suffix < *best) {
            *best = suffix;
            *found = true;
        }
        return;
    }

    for (int d = 0; d <= 9; ++d) {
        uint32_t mask = 1u << d;
        if (used & mask) continue;
        /* Leading zero is allowed only when n==1. */
        if (n > 1 && pos == n - 1 && d == 0) continue;

        seq_t new_suffix = (seq_t)d * pow10[pos] + suffix;
        if (new_suffix % (seq_t)(pos + 1) == 0) {
            dfs(pos + 1, n, used | mask, new_suffix, best, found, pow10);
        }
    }
}

static seq_t compute_n(idx_t n)
{
    /* pow10[i] = 10^i for i=0..n-1 */
    static seq_t pow10[11];
    pow10[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        pow10[i] = pow10[i - 1] * (seq_t)10;

    seq_t best = 0;
    bool found = false;
    dfs(0, n, 0, (seq_t)0, &best, &found, pow10);
    return found ? best : (seq_t)0;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* OEIS offset is 1 */
    if (n > 10) return 0;          /* only 10 distinct decimal digits exist */

    static bool computed[11] = {0};
    static seq_t cache[11]    = {0};

    if (!computed[n]) {
        cache[n] = compute_n(n);
        computed[n] = true;
    }
    return cache[n];
}
