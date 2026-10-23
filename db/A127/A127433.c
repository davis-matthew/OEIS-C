/*
 * A127433  a(1)=1. a(n) = number of earlier terms a(k), 1<=k<=n-1, such that
 *          (k+n) is coprime to a(k).
 * https://oeis.org/A127433
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n>1, a(n) = |{k | 1 ≤ k < n, gcd(k+n, a(k)) = 1}|
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/gcd.h"

static seq_t *seq_cache = NULL;
static size_t seq_cap  = 0;
static idx_t seq_last  = 0;

/* Grow the cache to hold at least 'need' indices. */
static void ensure_cache(idx_t need) {
    if (need < seq_cap)
        return;
    size_t new_cap = (seq_cap == 0) ? 8 : seq_cap << 1;
    while (new_cap <= (size_t)need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    if (seq_cache) {
        memcpy(tmp, seq_cache, seq_cap * sizeof(seq_t));
        free(seq_cache);
    }
    seq_cache = tmp;
    seq_cap  = new_cap;
}

seq_t a(idx_t n) {
    if (n == 0)           /* safety; sequence starts at index 1 */
        return 0;
    ensure_cache(n);
    for (idx_t m = seq_last + 1; m <= n; ++m) {
        seq_t cnt = 0;
        if (m == 1) {
            cnt = 1;          /* a(1) = 1 */
        } else {
            for (idx_t k = 1; k < m; ++k) {
                seq_t sum = (seq_t)(k + m);
                if (gcd(sum, seq_cache[k]) == 1)
                    ++cnt;
            }
        }
        seq_cache[m] = cnt;
    }
    seq_last = n;
    return seq_cache[n];
}
