/*
 * A167673  Number of reduced words of length n in Coxeter group on 17
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^15 = I.
 * https://oeis.org/A167673
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A167673: number of reduced words of length n in the Coxeter
   group on 17 generators with relations S_i^2 = I and (S_i S_j)^15 = I.
   The ordinary generating function is
        (t^15 + 2 t^14 + 2 t^13 + … + 2 t + 1) /
        (1 - 15 t - 15 t^2 - … - 15 t^14 + 120 t^15).
   From this we obtain the following recurrence:
        a(0)=1,  a(1)=17, …
        a(15)=15·(a(14)+…+a(1)) - 120·a(0) + 1
        a(n)=15·(a(n-1)+…+a(n-14)) - 120·a(n-15)   for n>15.  */

#include <stdlib.h>

static const seq_t init_terms[15] = {
    1ULL, 17ULL, 272ULL, 4352ULL, 69632ULL,
    1114112ULL, 17825792ULL, 285212672ULL,
    4563402752ULL, 73014444032ULL,
    1168231104512ULL, 18691697672192ULL,
    299067162755072ULL, 4785074604081152ULL,
    76561193665298432ULL
};

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t last_computed = (idx_t)-1;

    /* Allocate cache large enough for n+1 elements. */
    if (cache == NULL || n + 1 > cache_size) {
        idx_t new_size = n + 1;
        if (new_size < 15) new_size = 15;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;                /* allocation failure */
        cache = tmp;
        cache_size = new_size;
    }

    /* Initialise the first 15 terms if the cache has not yet been filled. */
    if (last_computed < 14) {
        for (idx_t i = last_computed + 1; i <= 14 && i <= n; ++i)
            cache[i] = init_terms[i];
        if (last_computed < 14) last_computed = 14;
    }

    /* Compute missing terms up to n. */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        if (i < 15) {                      /* safety, should not occur */
            cache[i] = init_terms[i];
        } else if (i == 15) {
            seq_t sum = 0;
            for (idx_t k = 1; k <= 14; ++k)
                sum += (seq_t)15 * cache[i - k];
            cache[i] = sum - (seq_t)120 * cache[0] + 1;
        } else {
            seq_t sum = 0;
            for (idx_t k = 1; k <= 14; ++k)
                sum += (seq_t)15 * cache[i - k];
            cache[i] = sum - (seq_t)120 * cache[i - 15];
        }
    }
    last_computed = n;

    return cache[n];
}
