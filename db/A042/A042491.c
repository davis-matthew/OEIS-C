/*
 * A042491  Denominators of continued fraction convergents to sqrt(773).
 * https://oeis.org/A042491
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 2686036*a(n-11) + a(n-22) (n ≥ 22) Initial values:
 *            a(0)=1, a(1)=1, a(2)=5, a(3)=66, a(4)=71, a(5)=208, a(6)=487,
 *            a(7)=695, a(8)=9522, a(9)=38783, a(10)=48305, a(11)=2647253,
 *            a(12)=2695558, a(13)=13429485, a(14)=177278863, a(15)=190708348,
 *            a(16)=558695559, a(17)=1308099466, a(18)=1866795025...
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 18, u64 n <= 32, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t base[22] = {
    (seq_t)1, (seq_t)1, (seq_t)5, (seq_t)66, (seq_t)71,
    (seq_t)208, (seq_t)487, (seq_t)695, (seq_t)9522, (seq_t)38783,
    (seq_t)48305, (seq_t)2647253, (seq_t)2695558, (seq_t)13429485,
    (seq_t)177278863, (seq_t)190708348, (seq_t)558695559,
    (seq_t)1308099466, (seq_t)1866795025, (seq_t)25576434791,
    (seq_t)104172534189, (seq_t)129748968980
};

static seq_t *cache = NULL;
static size_t cache_len = 0;
static idx_t last_computed = 21;

seq_t a(idx_t n) {
    if (n < 22) return base[n];

    /* Allocate or extend cache */
    if (cache_len <= (size_t)n) {
        size_t new_len = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (new_cache == NULL) abort();  /* allocation failure */

        /* Copy existing values if any */
        if (cache != NULL) {
            size_t copy_len = cache_len < new_len ? cache_len : new_len;
            for (size_t i = 0; i < copy_len; ++i)
                new_cache[i] = cache[i];
            free(cache);
        }

        /* Ensure base values are present */
        for (size_t i = 0; i < 22 && i < new_len; ++i)
            new_cache[i] = base[i];

        cache = new_cache;
        cache_len = new_len;
    }

    /* Compute missing terms up to n */
    if (n > last_computed) {
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            cache[(size_t)i] =
                (seq_t)2686036 * cache[(size_t)(i - 11)] +
                cache[(size_t)(i - 22)];
        }
        last_computed = n;
    }

    return cache[(size_t)n];
}
