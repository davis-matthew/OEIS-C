/*
 * A158242  Smallest n-digit number with distinct digits such that every
 *          k-digit substring (k <= n) taken from the left is divisible by k
 *          (k = 1..n).
 * https://oeis.org/A158242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) = smallest n‑digit number with distinct digits such that
 *            every left prefix of length k (1 ≤ k ≤ n) is divisible by k
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers */
#include <stdbool.h>

/* Recursive helper flag for building the sequence */
static bool dfs(idx_t pos, idx_t n, uint16_t used, seq_t val, seq_t *sol)
{
    if (pos == n) {
        *sol = val;
        return true;
    }

    int start = (pos == 0) ? 1 : 0;          /* first digit cannot be 0 */
    for (int d = start; d <= 9; ++d) {
        if (used & (1 << d))
            continue;

        seq_t nxt = val * 10 + d;
        int newlen = (int)(pos + 1);        /* length after adding this digit */
        if (nxt % newlen == 0) {
            if (dfs(pos + 1, n, used | (1 << d), nxt, sol))
                return true;
        }
    }
    return false;
}

seq_t a(idx_t n)
{
    /* The requirement of distinct digits bounds n to at most 10.  For any n>10
       there is no such number, so return 0.  The function is memoised for
       efficiency. */
    if (n < 1 || n > 10)
        return 0;

    static seq_t cache[11] = {0};
    if (cache[n] != 0)
        return cache[n];

    seq_t result = 0;
    dfs(0, n, 0, 0, &result);
    cache[n] = result;
    return result;
}
