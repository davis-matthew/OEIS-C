/*
 * A165645  Number of reduced words of length n in Coxeter group on 33
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^9 = I.
 * https://oeis.org/A165645
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n ≥ 1 a(n) = (n==9 ? 1 : (n<9 ? 2 : 0)) + 31 *
 *            (a(n-1)+…+a(n-8)) – 496 * a(n-9) (with a(k)=0 for k<0)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This recurrence follows from the generating function
   (t^9+2t^8+⋯+2t+1)/(496t^9−31t^8−⋯−31t+1). */

#include <stdlib.h>

static seq_t *cache = NULL;          /* cached sequence values */
static idx_t cached_size = 0;        /* number of values already cached */

/* ensure cache contains values up to index n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (n + 1 <= cached_size)
        return;                       /* nothing to do */

    /* choose new size: at least n+1, and grow geometrically */
    idx_t new_size = n + 1;
    if (new_size < 10)
        new_size = 10;                /* need space for the first 10 terms */
    if (cached_size > 0 && new_size < cached_size * 2)
        new_size = cached_size * 2;

    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp)
        abort();                      /* out of memory */
    cache = tmp;

    /* start generating missing terms */
    /* the first term a(0) is set once, when the cache is empty */
    if (cached_size == 0)
        cache[0] = (seq_t)1;

    for (idx_t i = cached_size; i < new_size; ++i) {
        /* term contributed by the numerator of the GF */
        seq_t term;
        if (i == 0)
            term = (seq_t)1;
        else if (i == 9)
            term = (seq_t)1;
        else if (i < 9)
            term = (seq_t)2;
        else
            term = (seq_t)0;

        seq_t sum = term;

        /* add 31 * a(i-k) for k = 1 .. 8 */
        for (int k = 1; k <= 8 && i >= (idx_t)k; ++k)
            sum += (seq_t)31 * cache[i - k];

        /* subtract 496 * a(i-9) when i ≥ 9 */
        if (i >= 9)
            sum -= (seq_t)496 * cache[i - 9];

        cache[i] = sum;
    }

    cached_size = new_size;
}

/* public interface required by the build */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
