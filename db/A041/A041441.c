/*
 * A041441  Denominators of continued fraction convergents to sqrt(236).
 * https://oeis.org/A041441
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 1123598*a(n-12) - a(n-24), a(0) = 1, a(1) = 2, a(2) = 3,
 *            a(3) = 11, a(4) = 58, a(5) = 69, a(6) = 472, a(7) = 541, a(8) =
 *            3177, a(9) = 10072, a(10) = 13249, a(11) = 36570, a(12) =
 *            1110349, a(13) = 2257268, a(14) = 3367617, a(15) = 12360119,
 *            a(16) = 65168212, a(17) = 77528331, a(18) = 530338...
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 20, u64 n <= 39, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* initial 24 terms */
static const seq_t init_vals[24] = {
    1ULL, 2ULL, 3ULL, 11ULL, 58ULL, 69ULL,
    472ULL, 541ULL, 3177ULL, 10072ULL, 13249ULL, 36570ULL,
    1110349ULL, 2257268ULL, 3367617ULL, 12360119ULL,
    65168212ULL, 77528331ULL, 530338198ULL, 607866529ULL,
    3569670843ULL, 11316879058ULL, 14886549901ULL, 41089978860ULL
};

/* cache for computed terms */
static seq_t *cache = NULL;          /* array of computed values */
static size_t cache_cap = 0;         /* current capacity of cache */
static idx_t last_computed = 23;     /* highest index computed so far */

/* a(n) returns the nth term of the sequence */
seq_t a(idx_t n)
{
    /* handle the first 24 terms directly */
    if (n < 24) {
        return init_vals[(idx_t)n];
    }

    /* allocate or grow the cache as needed */
    if (cache == NULL) {
        cache_cap = (size_t)(n + 1);
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);          /* allocation failed */
        for (idx_t i = 0; i < 24; ++i) {
            cache[i] = init_vals[i];
        }
        last_computed = 23;
    } else if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)calloc(new_cap, sizeof(seq_t));
        if (!new_cache) exit(EXIT_FAILURE);     /* allocation failed */
        /* copy existing values */
        for (size_t i = 0; i < cache_cap; ++i) {
            new_cache[i] = cache[i];
        }
        free(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        cache[i] = cache[i - 12] * (seq_t)1123598u - cache[i - 24];
    }
    last_computed = n;

    return cache[n];
}
