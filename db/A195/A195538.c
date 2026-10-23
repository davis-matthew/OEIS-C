/*
 * A195538  Denominators a(n) of Pythagorean approximations b(n)/a(n) to
 *          sqrt(8).
 * https://oeis.org/A195538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = 35*a(n-2) - 35*a(n-4) + a(n-6) for n ≥ 7 Initial values
 *            (offset 1): a(1) = 5 a(2) = 12 a(3) = 145 a(4) = 420 a(5) = 4901
 *            a(6) = 14280
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t base_vals[6] = {
    (seq_t)5,
    (seq_t)12,
    (seq_t)145,
    (seq_t)420,
    (seq_t)4901,
    (seq_t)14280
};

static seq_t *cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* safety for n < 1, not used by OEIS */
    if (n <= 6) return base_vals[n - 1];

    if (n > cache_len) {
        /* grow cache to size n */
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * n);
        if (!tmp) return (seq_t)0;        /* allocation failure: return 0 */
        cache = tmp;

        /* compute new entries */
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            if (i <= 6) {
                cache[i - 1] = base_vals[i - 1];
            } else {
                seq_t v = (seq_t)35 * cache[i - 3];   /* 35*a(i-2) */
                v -= (seq_t)35 * cache[i - 5];        /* -35*a(i-4) */
                v += cache[i - 7];                    /* +a(i-6)   */
                cache[i - 1] = v;
            }
        }
        cache_len = n;
    }

    return cache[n - 1];
}
