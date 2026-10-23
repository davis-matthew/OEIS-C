/*
 * A167653  Number of reduced words of length n in Coxeter group on 8
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^15 = I.
 * https://oeis.org/A167653
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/*
 * Recurrence: a(0..15) are given below.
 * For n ≥ 16:  a(n) = 6*Σ_{k=1}^{14} a(n-k) – 21*a(n-15)
 *
 * Initial values:
 *  a(0) =  1
 *  a(1) =  8
 *  a(2) = 56
 *  a(3) = 392
 *  a(4) = 2744
 *  a(5) = 19208
 *  a(6) = 134456
 *  a(7) = 941192
 *  a(8) = 6588344
 *  a(9) = 46118408
 *  a10 = 322828856
 *  a11 = 2259801992
 *  a12 = 15818613944
 *  a13 = 110730297608
 *  a14 = 775112083256
 *  a15 = 5425784582764
 */

static const seq_t init_vals[16] = {
    (seq_t)1ULL,
    (seq_t)8ULL,
    (seq_t)56ULL,
    (seq_t)392ULL,
    (seq_t)2744ULL,
    (seq_t)19208ULL,
    (seq_t)134456ULL,
    (seq_t)941192ULL,
    (seq_t)6588344ULL,
    (seq_t)46118408ULL,
    (seq_t)322828856ULL,
    (seq_t)2259801992ULL,
    (seq_t)15818613944ULL,
    (seq_t)110730297608ULL,
    (seq_t)775112083256ULL,
    (seq_t)5425784582764ULL
};

static seq_t *cache = NULL;    /* cache of computed values */
static size_t cache_cap = 0;    /* number of elements stored in cache */

/* external linkage as requested */
seq_t a(idx_t n)
{
    /* Grow the cache if necessary */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache)
            return (seq_t)0; /* allocation failed – caller will never use this value */

        /* copy any already stored values */
        if (cache) {
            memcpy(new_cache, cache, cache_cap * sizeof(seq_t));
            free(cache);
        }

        /* fill the new entries */
        for (idx_t i = cache_cap; i < (idx_t)new_cap; ++i) {
            if (i < 16) {
                new_cache[(size_t)i] = init_vals[(size_t)i];
            } else {
                seq_t sum = 0;
                for (idx_t k = 1; k <= 14; ++k)
                    sum += new_cache[(size_t)(i - k)];
                seq_t val = (seq_t)6 * sum - (seq_t)21 * new_cache[(size_t)(i - 15)];
                new_cache[(size_t)i] = val;
            }
        }

        cache = new_cache;
        cache_cap = new_cap;
    }

    return cache[(size_t)n];
}
