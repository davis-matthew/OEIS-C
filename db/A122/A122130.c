/*
 * A122130  Expansion of f(-x^4, -x^16) / psi(-x) in powers of x where psi()
 *          is a Ramanujan theta function and f(, ) is Ramanujan's general
 *          theta function.
 * https://oeis.org/A122130
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A122130
   a(n) = number of partitions of n into parts that are odd or
   congruent to 8 or 12 modulo 20.  Computed with dynamic programming:
   dp[0] = 1; for each allowed part s ≤ n, for m = s .. n, dp[m] += dp[m-s].
   The result is stored in a static cache that is recomputed when a larger
   index is requested.  This algorithm runs in O(n · #parts) ≈ O(n²/2),
   which is well below the 2 s limit for n up to about 10 000. */
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

static bool allowed_part(idx_t s)
{
    if (s == 0) return false;
    if ((s & 1) != 0) return true;            /* odd part          */
    if (s % 20 == 8) return true;             /* ≡ 8 (mod 20)      */
    if (s % 20 == 12) return true;            /* ≡ 12 (mod 20)     */
    return false;
}

/* Memoised DP array */
static seq_t *dp          = NULL;
static idx_t  max_n       = 0;   /* largest n for which dp is valid */

seq_t a(idx_t n)
{
    /* Allocate or grow the cache to size n+1 */
    if (dp == NULL) {
        dp = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
        if (dp == NULL) abort();
        max_n = n;
    } else if (n > max_n) {
        seq_t *tmp = (seq_t *)realloc(dp, (size_t)(n + 1) * sizeof(seq_t));
        if (tmp == NULL) abort();
        dp = tmp;
        max_n = n;
    }

    /* Re‑compute dp[0..n] from scratch whenever we extend the range.
       This guarantees correctness even when calls are out of order. */
    memset(dp, 0, (size_t)(max_n + 1) * sizeof(seq_t));
    dp[0] = (seq_t)1;

    for (idx_t s = 1; s <= max_n; ++s) {
        if (!allowed_part(s)) continue;
        for (idx_t m = s; m <= max_n; ++m) {
            dp[m] += dp[m - s];
        }
    }

    return dp[n];
}
