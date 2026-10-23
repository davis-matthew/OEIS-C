/*
 * A030540  Largest n-digit norep emirp.
 * https://oeis.org/A030540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) = largest n‑digit distinct‑digit emirp (0 if none)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 4, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Depth‑first search generates n‑digit numbers with distinct digits in
   descending order; the first that satisfies both primality tests is
   returned.  The search uses the supplied primality routine is_prime_big. */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* Recursive search helper. */
static bool dfs_emirp(int pos, int n, int *dig, bool *used, seq_t *res)
{
    if (pos == n) {
        seq_t num = 0;
        for (int i = 0; i < n; ++i)     /* build the number */
            num = num * 10 + (seq_t)dig[i];
        seq_t rev = 0;                   /* its decimal reversal */
        for (int i = n - 1; i >= 0; --i)
            rev = rev * 10 + (seq_t)dig[i];
        if (is_prime_big(num) && is_prime_big(rev)) {
            *res = num;
            return true;
        }
        return false;
    }
    for (int d = 9; d >= 0; --d) {
        if (pos == 0 && d == 0) continue;    /* no leading zero */
        if (!used[d]) {
            used[d] = true;
            dig[pos] = d;
            if (dfs_emirp(pos + 1, n, dig, used, res))
                return true;
            used[d] = false;
        }
    }
    return false;
}

/* Compute the largest n‑digit distinct‑digit emirp. */
static seq_t solve_emirp(idx_t n)
{
    if (n == 0 || n > 10) return 0;           /* no such number for other n */
    int digits[10];
    bool used[10] = { false };
    seq_t result = 0;
    dfs_emirp(0, (int)n, digits, used, &result);
    return result;
}

/* Public entry point for the OEIS algorithm. */
seq_t a(idx_t n)
{
    static seq_t cache[11] = {0};
    static bool done[11] = {false};

    if (n == 0 || n > 10) return 0;           /* undefined for these n */
    if (!done[(int)n]) {
        cache[(int)n] = solve_emirp(n);
        done[(int)n] = true;
    }
    return cache[(int)n];
}
