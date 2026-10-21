/*
 * A099215  a(n) = 4*a(n-1) - 4*a(n-2) + 3*a(n-3).
 * https://oeis.org/A099215
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*a(n-1) - 4*a(n-2) + 3*a(n-3) Recurrence with initial
 *            values a(0)=1, a(1)=2, a(2)=4
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *seq_cache = NULL;
static idx_t cache_size = 0;

/* ensure cache holds values up to n */
static void ensure_cache(idx_t n) {
    if (n < cache_size) return;
    if (seq_cache == NULL) {
        seq_cache = (seq_t*)malloc((n + 1) * sizeof(seq_t));
        if (!seq_cache) return;
        cache_size = n + 1;
    } else {
        seq_cache = (seq_t*)realloc(seq_cache, (n + 1) * sizeof(seq_t));
        if (!seq_cache) return;
        cache_size = n + 1;
    }
    /* base values */
    if (cache_size > 0) seq_cache[0] = (seq_t)1;
    if (cache_size > 1) seq_cache[1] = (seq_t)2;
    if (cache_size > 2) seq_cache[2] = (seq_t)4;
    /* compute new values */
    for (idx_t i = 3; i <= n; ++i) {
        seq_cache[i] = 4 * seq_cache[i-1] - 4 * seq_cache[i-2] + 3 * seq_cache[i-3];
    }
}

seq_t a(idx_t n) {
    ensure_cache(n);
    return seq_cache[n];
}
