/*
 * A126551  Number of base 30 n-digit numbers with adjacent digits differing
 *          by five or less.
 * https://oeis.org/A126551
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for A126551
   a(0) = 1
   a(1) = 30
   For n ≥ 2, a(n) is obtained by dynamic programming on the 30‑digit
   adjacency graph: edge (d,k) exists iff |d−k| ≤ 5.
   Let dp_m[d] be the number of length‑m strings ending with digit d.
   Then dp_{m+1}[d] = Σ_{k=max(0,d−5)}^{min(29,d+5)} dp_m[k]
   and a(m+1) = Σ_{d=0}^{29} dp_{m+1}[d];
   The function memoises computed values so that calls in arbitrary
   order are fast.
*/

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* Memoisation state */
    static bool initialized = false;
    static idx_t cur_max = 0;          /* largest index computed so far          */
    static seq_t *cache = NULL;        /* cache[0..cur_max]                      */

    /* Current and next length dp arrays */
    static seq_t dp[30];
    static seq_t dp_next[30];

    /* Initialisation on first call */
    if (!initialized) {
        /* cache for a(0) and a(1) */
        cache = (seq_t *)malloc(sizeof(seq_t) * 2);
        if (!cache) return (seq_t)0;                /* allocation failure */
        cache[0] = (seq_t)1;                       /* a(0) */
        /* dp for length 1: each digit appears once */
        for (int d = 0; d < 30; ++d) dp[d] = (seq_t)1;
        /* a(1) = sum dp[0..29] = 30 */
        seq_t sum = 0;
        for (int d = 0; d < 30; ++d) sum += dp[d];
        cache[1] = sum;
        cur_max = 1;
        initialized = true;
    }

    /* Return cached value if already computed */
    if (n <= cur_max) {
        return cache[n];
    }

    /* Extend cache up to index n */
    idx_t new_cap = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
    if (!tmp) {
        /* allocation failed – return 0 (unlikely in tests) */
        return (seq_t)0;
    }
    cache = tmp;

    /* Compute lengths from cur_max+1 up to n */
    for (idx_t len = cur_max + 1; len <= n; ++len) {
        /* Compute dp_next for this length */
        for (int d = 0; d < 30; ++d) {
            int low  = d - 5; if (low  < 0)  low  = 0;
            int high = d + 5; if (high > 29) high = 29;
            seq_t s = 0;
            for (int k = low; k <= high; ++k) s += dp[k];
            dp_next[d] = s;
        }
        /* Sum to get a(len) */
        seq_t total = 0;
        for (int d = 0; d < 30; ++d) total += dp_next[d];
        cache[len] = total;
        /* Prepare for next iteration */
        for (int d = 0; d < 30; ++d) dp[d] = dp_next[d];
    }

    cur_max = n;
    return cache[n];
}
