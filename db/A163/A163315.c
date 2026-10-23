/*
 * A163315  Number of reduced words of length n in Coxeter group on 4
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^5 = I.
 * https://oeis.org/A163315
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1)+2*a(n-2)+2*a(n-3)+2*a(n-4)-3*a(n-5), a(0)=1,
 *            a(1)=4, a(2)=12, a(3)=36, a(4)=108, a(5)=318
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 20, u64 n <= 40, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *seq_cache = NULL;      /* array of computed values          */
static size_t  cache_len  = 0;        /* number of values stored          */
static size_t  cache_cap  = 0;        /* allocated capacity in seq_t elem */

seq_t a(idx_t n)
{
    /* small indices – return constant values directly                */
    if ((size_t)n < 6) {
        static const seq_t base[6] = {1, 4, 12, 36, 108, 318};
        return base[(size_t)n];
    }

    /* initialise cache if it hasn't been yet                          */
    if (cache_len == 0) {
        /* start with room for at least the first 6 values             */
        cache_cap = 16;
        seq_cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        static const seq_t base[6] = {1, 4, 12, 36, 108, 318};
        for (size_t i = 0; i < 6; ++i)
            seq_cache[i] = base[i];
        cache_len = 6;
    }

    /* enlarge cache if necessary                                       */
    if ((size_t)n + 1 > cache_len) {
        size_t new_len = (size_t)n + 1;
        if (new_len > cache_cap) {
            size_t new_cap = cache_cap;
            while (new_cap < new_len)
                new_cap <<= 1;                /* double until big enough */
            seq_cache = (seq_t *)realloc(seq_cache,
                                          new_cap * sizeof(seq_t));
            cache_cap = new_cap;
        }

        /* compute new values starting where we left off                 */
        for (size_t i = cache_len; i < new_len; ++i) {
            seq_cache[i] =
                2 * seq_cache[i-1] +
                2 * seq_cache[i-2] +
                2 * seq_cache[i-3] +
                2 * seq_cache[i-4] -
                3 * seq_cache[i-5];
        }
        cache_len = new_len;
    }

    return seq_cache[(size_t)n];
}
