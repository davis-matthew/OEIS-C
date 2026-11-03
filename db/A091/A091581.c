/*
 * A091581  Number of partitions of n into distinct decimal palindromes.
 * https://oeis.org/A091581
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of partitions of n into distinct decimal
 *            palindromes
 * Range:     u8 n <= 103, u16 n <= 450, u32 n <= 1773, u64 n <= 7093,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 86 terms through n = 9763 (2026-11-03)
 * Helpers:   is_palindrome_base, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses a classic 0‑1 coin–change dynamic programming. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_palindrome_base.h"
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* The static table holds the DP values for all n that have been
       computed so far.  It grows only when a larger n is requested. */
    static seq_t *dp = NULL;
    static size_t dp_cap = 0;

    /* Convert the index to a type that can be used for array indexing. */
    size_t sn = (size_t)n;

    /* Ensure the static array is large enough. */
    if (dp_cap <= sn) {
        seq_t *tmp = mem_resize(seq_t, dp, (sn + 1));
        if (!tmp) return (seq_t)0;      /* allocation failed – return 0 */
        dp   = tmp;
        dp_cap = sn + 1;
    }

    /* Initialise DP: there is exactly one way to form 0 (empty partition). */
    dp[0] = (seq_t)1;
    for (size_t i = 1; i <= sn; ++i) dp[i] = (seq_t)0;

    /* Enumerate all decimal palindromes p ≤ n.  Each palindrome is used
       at most once (distinct summands) by iterating the DP array backwards. */
    for (idx_t p = 1; p <= n; ++p) {
        if (!is_palindrome_base((seq_t)p, 10))
            continue;
        size_t sp = (size_t)p;
        for (size_t i = sn; i >= sp; --i) {
            dp[i] += dp[i - sp];
            if (i == sp) break;           /* avoid unsigned overflow */
        }
    }

    return dp[sn];
}
