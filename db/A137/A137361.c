/*
 * A137361  a(n) = Sum_{k=0..n/2} k*binomial(n-2*k, 3*k+2).
 * https://oeis.org/A137361
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} k*binomial(n-2*k, 3*k+2)
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 39, u64 n <= 80, u128 n <= 162,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

static seq_t *cache = NULL;
static idx_t cache_size = 0;

/* Compute a single value of the sequence directly from the definition */
static inline seq_t compute_value(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n / 2; ++k) {
        idx_t k3b = 3 * k + 2;        /* 3*k + 2 */
        idx_t nmk = n - 2 * k;        /* n - 2*k */
        if (k3b > nmk)               /* binomial will be zero beyond this point */
            break;
        seq_t bin = binomial(nmk, k3b); /* chosen from utils */
        sum += bin * (seq_t)k;
    }
    return sum;
}

/* External sequence function */
seq_t a(idx_t n)
{
    if (n < cache_size)
        return cache[n];

    idx_t newsize = n + 1;
    cache = (seq_t *)realloc(cache, newsize * sizeof(seq_t)); /* cast to avoid C++ warning */

    for (idx_t i = cache_size; i < newsize; ++i)
        cache[i] = compute_value(i);

    cache_size = newsize;
    return cache[n];
}
