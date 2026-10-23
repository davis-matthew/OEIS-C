/*
 * A041506  Numerators of continued fraction convergents to sqrt(270).
 * https://oeis.org/A041506
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 13, u64 n <= 27, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A041506: Numerators of continued fraction convergents to sqrt(270).
    Recurrence:
        p_-1 = 1,   p_0 = 16
        p_k   = a_k * p_{k-1} + p_{k-2}      (k ≥ 1)
    where the continued‑fraction coefficients repeat with period
        [2, 3, 6, 3, 2, 32].
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Continued–fraction terms for sqrt(270) */
    static const unsigned int period[6] = {2u, 3u, 6u, 3u, 2u, 32u};
    const unsigned int a0 = 16u;                 /* a[0] */

    /* Memoisation cache: cache[k] == p_k  (0‑based) */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;                 /* number of stored p_k */
    static size_t cache_cap = 0;                 /* allocated capacity */

    /* a(0) */
    if (n == 0) return (seq_t)a0;

    /* First allocation: at least room for p_0 */
    if (cache_len == 0) {
        cache_cap = 2;                           /* small start size */
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) { /* allocation failed */ exit(EXIT_FAILURE); }
        cache[0] = (seq_t)a0;                    /* p_0 */
        cache_len = 1;
    }

    /* Ensure capacity for index n */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (cache_cap > 0) ? cache_cap : 1;
        while (new_cap <= (size_t)n + 1) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) { /* allocation failed */ exit(EXIT_FAILURE); }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing convergents up to n */
    for (size_t k = cache_len; k <= (size_t)n; ++k) {
        unsigned int ak = period[(k - 1) % 6];    /* a_k for k ≥ 1 */
        seq_t prev1 = cache[k - 1];
        seq_t prev2 = (k >= 2) ? cache[k - 2] : (seq_t)1;
        cache[k] = prev1 * ak + prev2;
    }
    cache_len = (size_t)n + 1;

    return cache[(size_t)n];
}
