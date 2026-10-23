/*
 * A109075  Number of primes which use each of 0-to-n decimal digits exactly
 *          once.
 * https://oeis.org/A109075
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, full, nonn, fini
 * Formula:   a(n) = number of primes formed by permutations of the digits
 *            0..n, * each used exactly once, with no leading zero. * The
 *            function computes this directly for n ≤ 9 (larger n are
 *            impossible because * decimal digits are only 0–9) and returns 0
 *            otherwise. It also skips all n for * which the digit sum...
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static void dfs_rest(idx_t depth, uint64_t cur, uint64_t used_mask,
                     idx_t m, seq_t *cnt)
{
    if (depth == m) {
        if (is_prime(cur))
            ++(*cnt);
        return;
    }
    for (idx_t d = 0; d < m; ++d) {
        if (used_mask & (1ULL << d))
            continue;
        dfs_rest(depth + 1,
                 cur * 10 + d,
                 used_mask | (1ULL << d),
                 m,
                 cnt);
    }
}

seq_t a(idx_t n)
{
    /* Only decimal digits 0–9 exist */
    if (n > 9)
        return (seq_t)0;

    /* If the sum of digits is a multiple of 3, all permutations are
     * divisible by 3 and cannot be prime. */
    seq_t sum = (seq_t)n * ((seq_t)n + 1) / 2;
    if (sum % 3 == 0)
        return (seq_t)0;

    idx_t m = n + 1;          /* number of digits */
    seq_t count = 0;

    /* Choose first digit: must be non‑zero */
    for (idx_t d = 1; d < m; ++d) {
        dfs_rest(1,
                 d,
                 1ULL << d,
                 m,
                 &count);
    }

    return count;
}
