/*
 * A126533  Number of base 12 n-digit numbers with adjacent digits differing
 *          by five or less.
 * https://oeis.org/A126533
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of base‑12 n‑digit strings whose adjacent digits
 *            differ by at most 5. Recurrence (dynamic programming) for n ≥ 2:
 *            let dp[i][d] = number of length‑i strings ending in digit d (0 ≤
 *            d ≤ 11). dp[1][d] = 1 for all d. dp[i+1][d] = Σ_{e : |e‑d| ≤ 5}
 *            dp[i][e]. Then a(n) = Σ_d dp[n][d] with...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Cache the sequence values up to the largest n requested so far. */
    static seq_t *cache = NULL;   /* cache[0] = a(0), cache[1] = a(1), ... */
    static idx_t cache_len = 0;   /* number of elements currently cached */

    /* DP arrays holding counts for sequences of the current length. */
    static seq_t dp_cur[12];
    static seq_t dp_next[12];
    static idx_t dp_len = 0;      /* length of sequences represented by dp_cur */

    /* Initialisation on the first call. */
    if (cache_len == 0)
    {
        cache = NULL;
        cache_len = 2;              /* reserve for a(0) and a(1) */
        cache = (seq_t *)calloc((size_t)cache_len, sizeof(seq_t));
        if (!cache) return 0;       /* allocation failure: return 0 */

        cache[0] = (seq_t)1;        /* a(0) */
        cache[1] = (seq_t)12;       /* a(1) */

        /* dp_cur corresponds to length 1: one way for each ending digit. */
        for (idx_t d = 0; d < 12; ++d) dp_cur[d] = (seq_t)1;
        dp_len = 1;
    }

    /* Return cached value if already computed. */
    if (n < cache_len) return cache[n];

    /* Extend the cache up to n. */
    while (cache_len <= n)
    {
        /* Compute dp_next for length dp_len + 1. */
        for (idx_t d = 0; d < 12; ++d)
        {
            seq_t sum = 0;
            idx_t left  = (d >= 5) ? (d - 5) : 0;
            idx_t right = (d + 5 < 12) ? (d + 5) : 11;
            for (idx_t e = left; e <= right; ++e)
                sum += dp_cur[e];
            dp_next[d] = sum;
        }

        /* Compute a(dp_len + 1) as the sum over all ending digits. */
        seq_t total = 0;
        for (idx_t d = 0; d < 12; ++d) total += dp_next[d];

        /* Store it in the cache. */
        cache = (seq_t *)realloc(cache, (size_t)(cache_len + 1) * sizeof(seq_t));
        cache[cache_len] = total;
        ++cache_len;

        /* Advance DP to the new length. */
        for (idx_t d = 0; d < 12; ++d) dp_cur[d] = dp_next[d];
        ++dp_len;
    }

    return cache[n];
}
