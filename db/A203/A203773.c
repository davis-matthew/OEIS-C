/*
 * A203773  Vandermonde sequence using x^2 + y^2 applied to
 *          (0,1,1,2,2,...,floor(n/2)).
 * https://oeis.org/A203773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{0 ≤ i < j < n} (⌊(i+1)/2⌋² + ⌊(j+1)/2⌋²)
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 5, u64 n <= 7, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache   = NULL;      /* stored values a(1)..a(k) in cache[0..k-1]   */
static size_t  cache_cap = 0;     /* current allocation size                       */
static size_t  cache_len = 0;     /* number of values already computed             */
static seq_t   product    = 1;     /* a(current length) stored during construction   */

seq_t a(idx_t n) {
    if (n == 0) return 0;                         /* undefined, but harmless */
    size_t target = (size_t)n;                    /* working in size_t domain */

    /* return cached value if already computed */
    if (cache_len >= target) return cache[target - 1];

    /* ensure allocation */
    if (target > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        while (new_cap < target) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* compute missing terms */
    for (size_t cur = cache_len + 1; cur <= target; ++cur) {
        seq_t tn = (seq_t)(cur / 2);              /* ⌊cur/2⌋ = ⌊(cur-1+1)/2⌋ = ⌊(cur-1+1)/2⌋ */
        if (cur == 1) {                           /* first term is 1 */
            product = 1;
        } else {
            /* multiply by new factors involving the new element tn */
            for (size_t i = 0; i < cur - 1; ++i) {
                seq_t ti = (seq_t)((i + 1) / 2);   /* ⌊(i+1)/2⌋ */
                seq_t factor = ti * ti + tn * tn;
                product *= factor;
            }
        }
        cache[cache_len] = product;               /* store a(cur) */
        cache_len++;
    }

    return cache[target - 1];
}
