/*
 * A166308  Number of reduced words of length n in Coxeter group on 47
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^10 = I.
 * https://oeis.org/A166308
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the linear recurrence * a(n) = 46*a(n-1) -
 *            1080*a(n-10) + 1035*a(n-11) for n ≥ 12 * with initial values
 *            (for n = 0 … 11) taken from the OEIS entry
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* ensure cache contains all terms up to n */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    size_t idx = (size_t)n;

    /* initial values for n = 0 … 11 (12 terms) */
    static const seq_t init[12] = {
        (seq_t)1ULL,
        (seq_t)47ULL,
        (seq_t)2162ULL,
        (seq_t)99452ULL,
        (seq_t)4574792ULL,
        (seq_t)210440432ULL,
        (seq_t)9680259872ULL,
        (seq_t)445291954112ULL,
        (seq_t)20483429889152ULL,
        (seq_t)942237774900992ULL,
        (seq_t)43342937645444551ULL,
        (seq_t)1993775131690399620ULL  /* a(11) */
    };

    /* first call: build cache initialized with first 12 terms */
    if (!cache) {
        cache = (seq_t *)malloc(12 * sizeof(seq_t));
        for (size_t i = 0; i < 12; ++i)
            cache[i] = init[i];
        cache_size = 12;
    }

    /* grow cache if necessary */
    if (idx + 1 > cache_size) {
        size_t new_size = idx + 1;
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        for (size_t i = cache_size; i <= idx; ++i) {
            /* compute via recurrence for i ≥ 12; earlier indices are preset */
            if (i >= 12) {
                seq_t term = (seq_t)46 * cache[i - 1];
                term -= (seq_t)1080 * cache[i - 10];
                term += (seq_t)1035 * cache[i - 11];
                cache[i] = term;
            }
        }
        cache_size = new_size;
    }

    return cache[idx];
}
