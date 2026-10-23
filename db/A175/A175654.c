/*
 * A175654  Eight bishops and one elephant on a 3 X 3 chessboard. G.f.: (1 - x
 *          - x^2)/(1 - 3*x - x^2 + 6*x^3).
 * https://oeis.org/A175654
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*a(n-1) + a(n-2) - 6*a(n-3), with a(0) = 1, a(1) = 2,
 *            a(2) = 6 (recurrence derived from the OEIS definition)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 25, u64 n <= 52, u128 n <= 105,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache to speed up arbitrary order accesses */
static seq_t *cache = NULL;      /* array of computed terms */
static idx_t cache_len = 0;      /* number of terms stored (indices 0 .. cache_len-1) */

/* expand cache to at least n+1 entries and compute missing terms */
static void ensure_cache(idx_t n)
{
    idx_t new_len = n + 1;
    if (new_len <= cache_len) return;

    /* reallocate array */
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) abort();              /* allocation failed */
    cache = tmp;

    /* compute terms that are missing */
    for (idx_t i = cache_len; i < new_len; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
        } else if (i == 1) {
            cache[1] = (seq_t)2;
        } else if (i == 2) {
            cache[2] = (seq_t)6;
        } else {
            cache[i] = (seq_t)3 * cache[i-1] + cache[i-2] - (seq_t)6 * cache[i-3];
        }
    }
    cache_len = new_len;
}

/* public function required by the specification */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
