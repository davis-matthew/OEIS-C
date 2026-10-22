/*
 * A006696  a(n) = min_{k=1..n} (a(k-1) + 2^k*(n + a(n-k))); a(0) = 0.
 * https://oeis.org/A006696
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = min_{k=1..n} (a(k-1) + 2^k*(n + a(n-k))), a(0) = 0
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n) {
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        cache[0] = 0;
        cache_len = 1;
    }
    if (n >= cache_len) {
        cache = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        for (idx_t m = cache_len; m <= n; m++) {
            /* k = 1 candidate */
            seq_t best = cache[0] + (seq_t)2 * ((seq_t)m + cache[m - 1]);
            seq_t p = 2;
            for (idx_t k = 2; k <= m; k++) {
                /* stop once 2^k alone exceeds the current best */
                if (p > best / 2) break;
                p = p * 2;
                seq_t base = cache[k - 1];
                if (base >= best) continue;
                seq_t mult = (seq_t)m + cache[m - k];
                /* need base + p*mult < best, checked without overflow */
                if (mult > (best - base) / p) continue;
                seq_t term = base + p * mult;
                if (term < best) best = term;
            }
            cache[m] = best;
        }
        cache_len = n + 1;
    }
    return cache[n];
}
