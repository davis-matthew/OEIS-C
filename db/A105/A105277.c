/*
 * A105277  Let F(n) denote the Fibonacci numbers, A000045: a(n) =
 *          Sum_{k=0..n} C(n,k)^2*(n-k)!*F(k).
 * https://oeis.org/A105277
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (4*n - 3)*a(n-1) - 2*(n-1)*(3*n - 5)*a(n-2) +
 *            (n-2)^2*(4*n - 7)*a(n-3) - (n-3)^2*(n-2)^2*a(n-4) with initial
 *            values a(0)=0, a(1)=1, a(2)=5, a(3)=29
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* cache to store computed terms */
static seq_t *cache = NULL;
static idx_t cache_size = 0;

/* extend the cache so that cache[0..upto] are computed */
static void extend_cache(idx_t upto)
{
    if (upto < cache_size) return;

    /* allocate enough space */
    idx_t new_size = upto + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) return;          /* allocation failed – give up */
    cache = new_cache;

    /* compute missing terms */
    for (idx_t i = cache_size; i <= upto; ++i) {
        seq_t val;
        if (i == 0)          val = (seq_t)0;
        else if (i == 1)     val = (seq_t)1;
        else if (i == 2)     val = (seq_t)5;
        else if (i == 3)     val = (seq_t)29;
        else {
            seq_t t1 = cache[i - 1];
            seq_t t2 = cache[i - 2];
            seq_t t3 = cache[i - 3];
            seq_t t4 = cache[i - 4];

            seq_t c1 = (seq_t)(4 * i - 3);
            seq_t c2 = (seq_t)(2 * (i - 1) * (3 * i - 5));
            seq_t c3 = (seq_t)((i - 2) * (i - 2) * (4 * i - 7));
            seq_t c4 = (seq_t)((i - 3) * (i - 3) * (i - 2) * (i - 2));

            val = c1 * t1 - c2 * t2 + c3 * t3 - c4 * t4;
        }
        cache[i] = val;
    }
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    extend_cache(n);
    return cache[n];
}
