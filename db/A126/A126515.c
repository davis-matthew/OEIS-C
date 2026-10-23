/*
 * A126515  Number of base 20 n-digit numbers with adjacent digits differing
 *          by four or less.
 * https://oeis.org/A126515
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  a(0)=1, a(1)=20;
   For n ≥ 2,
     a(n) = Σ_{e=0}^{19} Σ_{d:|d-e|≤4} a(n-1,d),
   where a(k,d) counts k‑digit base‑20 strings ending with digit d.
   This DP keeps the counts for each possible last digit. */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Cache of already computed values.  20001 is enough for n≈10000. */
    static seq_t cache[20001];
    static size_t cache_len = 1;   /* highest indexed value stored */

    /* Base cases. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)20;

    /* Initialise cache for 0 and 1 on first call. */
    if (cache_len == 1 && cache[1] == (seq_t)0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)20;
    }

    /* Return already computed value if possible. */
    if ((size_t)n <= cache_len)
        return cache[n];

    /* Dynamic programming arrays. */
    static seq_t dp_prev[20];   /* counts for sequences of current length */
    static seq_t dp_cur[20];
    static int dp_initialized = 0;
    if (!dp_initialized) {
        for (idx_t d = 0; d < 20; ++d)
            dp_prev[d] = (seq_t)1;      /* length 1 sequences */
        dp_initialized = 1;
    }

    /* Build values up to n. */
    for (idx_t len = (idx_t)(cache_len + 1); len <= n; ++len) {
        for (idx_t e = 0; e < 20; ++e) {
            idx_t lo = (e >= 4) ? e - 4 : 0;
            idx_t hi = (e + 4 < 20) ? e + 4 : 19;
            seq_t sum = (seq_t)0;
            for (idx_t d = lo; d <= hi; ++d)
                sum += dp_prev[d];
            dp_cur[e] = sum;
        }

        seq_t total = (seq_t)0;
        for (idx_t e = 0; e < 20; ++e)
            total += dp_cur[e];

        cache[len] = total;
        cache_len = (size_t)len;

        /* Prepare next iteration. */
        for (idx_t e = 0; e < 20; ++e)
            dp_prev[e] = dp_cur[e];
    }

    return cache[n];
}
