/*
 * A221145  a(n) is the number of permutations of n elements with exactly one
 *          fixed point and no transpositions.
 * https://oeis.org/A221145
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*A038205(n-1) = n * f(n-1) where f(k) counts
 *            permutations of k elements with all cycles of length >=3
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 2,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache of f(k) values */
static seq_t *f_cache = NULL;
static size_t f_size = 0;

/* compute f(k) for k = n-1 and return a(n) */
seq_t a(idx_t n)
{
    /* ensure f_cache is large enough for indices up to n-1 */
    size_t need = n;                       /* need indices 0 .. n-1 inclusive */
    if (f_size < need) {
        size_t new_size = need;
        seq_t *tmp = (seq_t *)realloc(f_cache, new_size * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);     /* allocation failed */
        f_cache = tmp;
        /* initialize base cases if this is the first allocation */
        if (f_size == 0) {
            f_cache[0] = 1;                /* f(0) = 1  (empty permutation) */
            if (new_size > 1) f_cache[1] = 0;  /* f(1) = 0   */
            if (new_size > 2) f_cache[2] = 0;  /* f(2) = 0   */
        }
        /* compute missing entries */
        size_t start = (f_size > 2) ? f_size : 3;
        for (size_t k = start; k < new_size; ++k) {
            seq_t t1 = (seq_t)(k-1) * f_cache[k-1];
            seq_t t2 = (seq_t)(k-1) * (seq_t)(k-2) * f_cache[k-3];
            f_cache[k] = t1 + t2;
        }
        f_size = new_size;
    }

    /* a(n) = n * f(n-1) */
    seq_t result = (seq_t)n * f_cache[n-1];
    return result;
}
