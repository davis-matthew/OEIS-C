/*
 * A063284  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          11 ).
 * https://oeis.org/A063284
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(n-4) + a(n-6) - a(n-10) for n>13, with initial values
 *            a(2) = -1, a(3) = 1, a(4) = 5, a(5) = 10, a(6) = 15, a(7) = 20,
 *            a(8) = 25, a(9) = 30, a(10)=35, a(11)=40, a(12)=45, a(13)=48
 * Range:     i8 n <= 29, i16 n <= 55, i32 n <= 55, i64 n <= 55, i128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_cap = 0;      /* allocated capacity (in indices) */
static idx_t computed_upto = 0;  /* highest index computed so far */

seq_t a(idx_t n)
{
    /* OEIS offset is 2; values for n<2 are not defined */
    if (n < 2)
        return 0;

    /* First call: allocate enough space for up to the requested n
       and fill in the base values. */
    if (!cache) {
        /* Ensure we have room for at least indices 0..13 to store base */
        cache_cap = (n > 13 ? n + 1 : 14);
        cache = (seq_t*)calloc((size_t)cache_cap, sizeof(seq_t));
        if (!cache)
            return 0;  /* allocation failed – return 0 as a safe value */

        /* Base values (indices 0 and 1 are unused) */
        cache[0] = 0; cache[1] = 0;
        cache[2] = (seq_t)-1;  cache[3] = (seq_t)1;
        cache[4] = (seq_t)5;   cache[5] = (seq_t)10;
        cache[6] = (seq_t)15;  cache[7] = (seq_t)20;
        cache[8] = (seq_t)25;  cache[9] = (seq_t)30;
        cache[10] = (seq_t)35; cache[11] = (seq_t)40;
        cache[12] = (seq_t)45; cache[13] = (seq_t)48;

        computed_upto = 13;
    }

    /* Grow the array if the requested n exceeds current capacity. */
    if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < (size_t)(n + 1))
            new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)  /* allocation failed */
            return 0;
        /* Zero the newly allocated part */
        for (size_t i = cache_cap; i < new_cap; ++i)
            tmp[i] = 0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n (if any). */
    if (n > computed_upto) {
        for (idx_t i = computed_upto + 1; i <= n; ++i) {
            if (i <= 13) {
                /* base cases already filled; skip */
                continue;
            }
            cache[(size_t)i] =
                cache[(size_t)(i - 4)] +
                cache[(size_t)(i - 6)] -
                cache[(size_t)(i - 10)];
        }
        computed_upto = n;
    }

    return cache[(size_t)n];
}
