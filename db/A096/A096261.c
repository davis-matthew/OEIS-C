/*
 * A096261  Number of n-tuples of 0,1,2,3,4,5,6,7,8,9 without consecutive
 *          digits.
 * https://oeis.org/A096261
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = 10*a(n-1) - 9*a(n-2) + 8*a(n-3) - 7*a(n-4) + 6*a(n-5) -
 *            5*a(n-6) + 4*a(n-7) - 3*a(n-8) + 2*a(n-9) - 1*a(n-10), a(0) = 1,
 *            a(n) = 0 for n < 0
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_size = 0;

/* compute a[i] assuming a[0..i-1] are already in cache */
static seq_t compute_at(size_t i)
{
    seq_t sum = 0;
    for (size_t k = 1; k <= 10; ++k) {
        if (i < k) continue;
        seq_t term  = cache[i - k];
        seq_t coeff = (seq_t)(11 - (int)k); /* 10,9,...,1 */
        if ((k & 1) == 0)        /* even k -> subtract */
            sum -= coeff * term;
        else                     /* odd k  -> add    */
            sum += coeff * term;
    }
    return sum;
}

seq_t a(idx_t n)
{
    size_t idx = (size_t)n;

    /* initialise cache with a(0) = 1 */
    if (cache_size == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache_size = 1;
    }

    /* extend cache if necessary */
    if (idx >= cache_size) {
        size_t new_size = idx + 1;
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        for (size_t i = cache_size; i < new_size; ++i)
            cache[i] = compute_at(i);
        cache_size = new_size;
    }

    return cache[idx];
}
