/*
 * A116787  Number of permutations of length n which avoid the patterns 1432,
 *          2314, 3124.
 * https://oeis.org/A116787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=2, ..., a(9)=7369. Recurrence for n ≥ 10: a(n) =
 *            8a(n‑1) – 25a(n‑2) + 40a(n‑3) – 32a(n‑4) + 7a(n‑5) + 4a(n‑6) –
 *            6a(n‑7) + 2a(n‑8) – a(n‑9)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 20, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache       = NULL;
static idx_t  cached_last = 0;
static size_t cache_cap   = 0;

seq_t a(idx_t n) {
    if (n == 0) return 0; /* not used */

    /* ensure array size */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap <= (size_t)(n + 1)) new_cap <<= 1;
        seq_t *new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (cache) {
            for (size_t i = 1; i <= cached_last; ++i) new_cache[i] = cache[i];
            free(cache);
        }
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* initial values a(1)…a(9) */
    static const seq_t init[10] = {
        0, /* a(0) unused */
        1, 2, 6, 21, 73, 240, 759, 2365, 7369
    };

    /* compute missing terms up to n */
    for (idx_t i = cached_last + 1; i <= n; ++i) {
        if (i <= 9) {
            cache[i] = init[i];
        } else {
            // Positive part
            seq_t p = 0;
            p += (seq_t)8   * cache[i - 1];
            p += (seq_t)40  * cache[i - 3];
            p += (seq_t)7   * cache[i - 5];
            p += (seq_t)4   * cache[i - 6];
            p += (seq_t)2   * cache[i - 8];

            // Negative part
            seq_t m = 0;
            m += (seq_t)25  * cache[i - 2];
            m += (seq_t)32  * cache[i - 4];
            m += (seq_t)6   * cache[i - 7];
            m +=              cache[i - 9];

            cache[i] = p - m;
        }
    }
    cached_last = n;
    return cache[n];
}
