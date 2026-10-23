/*
 * A041827  Denominators of continued fraction convergents to sqrt(434).
 * https://oeis.org/A041827
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 250*a(n-4) - a(n-8) for n > 7 Initial values: a(0)=1,
 *            a(1)=1, a(2)=5, a(3)=6, a(4)=245, a(5)=251, a(6)=1249, a(7)=1500
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static const seq_t init_vals[8] = {
    (seq_t)1ULL,      /* a(0) */
    (seq_t)1ULL,      /* a(1) */
    (seq_t)5ULL,      /* a(2) */
    (seq_t)6ULL,      /* a(3) */
    (seq_t)245ULL,    /* a(4) */
    (seq_t)251ULL,    /* a(5) */
    (seq_t)1249ULL,   /* a(6) */
    (seq_t)1500ULL    /* a(7) */
};

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;      /* dynamically‑grown cache */
    static size_t cap     = 0;         /* allocated size */
    static size_t cached  = 0;         /* computed values up to cached-1 */

    /* Grow cache if necessary */
    if (n + 1 > cap) {
        size_t new_cap = cap ? cap << 1 : 16;
        while (new_cap < n + 1) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) { /* allocation failed – abort */
            /* In a real program you might handle this differently */
            exit(1);
        }
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute missing values */
    for (size_t i = cached; i <= (size_t)n; ++i) {
        if (i < 8) {
            cache[i] = init_vals[i];
        } else {
            cache[i] = (seq_t)250 * cache[i - 4] - cache[i - 8];
        }
    }
    cached = n + 1;

    return cache[n];
}
