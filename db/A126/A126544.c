/*
 * A126544  Number of base 23 n-digit numbers with adjacent digits differing
 *          by five or less.
 * https://oeis.org/A126544
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A126544
 *  Formula: a(n) = number of length‑n base‑23 strings with
 *  adjacent digits differing by at most 5.
 *  Recurrence: let f(k,d) be the number of strings of length k
 *  ending with digit d (0 ≤ d ≤ 22). Then
 *    f(1,d) = 1,  and
 *    f(k+1,d) = Σ_{|d‑e|≤5} f(k,e).
 *  a(k) = Σ_{d=0}^{22} f(k,d).
 *
 *  The implementation uses dynamic programming with five‑state
 *  window constraints and memoises all previously computed terms.
 *  It is portable to both C11 and C++17.
 */
#include <stddef.h>   /* size_t */

static seq_t cache[10001];   /* cache[0]..cache[10000] */
static idx_t max_cached = 0; /* largest n whose value is in cache */
static seq_t dp_cur[23], dp_next[23]; /* DP arrays for digits 0..22 */
static int initialized = 0;          /* flag */

seq_t a(idx_t n)
{
    /* Initialise once on the first call. */
    if (!initialized) {
        /* cache[0] and cache[1] are the known first terms. */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)23;
        max_cached = 1;

        /* For strings of length 1 each digit occurs once. */
        for (int d = 0; d < 23; ++d) {
            dp_cur[d] = (seq_t)1;
            dp_next[d] = (seq_t)0;
        }

        initialized = 1;
    }

    /* If the requested n is already cached, return it. */
    if (n <= max_cached)
        return cache[n];

    /* For safety, limit to the pre‑allocated maximum. */
    if (n >= 10001)
        return (seq_t)0;   /* out of range – caller may handle as needed */

    /* Extend the cache up to the requested n. */
    for (idx_t len = max_cached + 1; len <= n; ++len) {
        /* Compute dp_next[d] for all digits d. */
        for (int d = 0; d < 23; ++d) {
            seq_t sum = (seq_t)0;
            int lo = d - 5;
            if (lo < 0) lo = 0;
            int hi = d + 5;
            if (hi > 22) hi = 22;
            for (int j = lo; j <= hi; ++j)
                sum += dp_cur[j];
            dp_next[d] = sum;
        }

        /* Sum over all ending digits to obtain a(len). */
        seq_t total = (seq_t)0;
        for (int d = 0; d < 23; ++d)
            total += dp_next[d];
        cache[len] = total;

        /* Prepare for the next iteration: dp_cur becomes dp_next. */
        for (int d = 0; d < 23; ++d)
            dp_cur[d] = dp_next[d];
    }

    max_cached = n;
    return cache[n];
}
