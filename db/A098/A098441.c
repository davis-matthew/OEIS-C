/*
 * A098441  Expansion of 1/sqrt(1 - 2*x - 63*x^2).
 * https://oeis.org/A098441
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} binomial(n-k, k) * binomial(n, k) * 16^k
 *            Recurrence: n*a(n) = (2*n-1)*a(n-1) + 63*(n-1)*a(n-2) Initial
 *            values: a(0) = 1, a(1) = 1 This implementation uses the
 *            recurrence but computes the intermediate products in a wider
 *            type when seq_t is 64‑bit or smaller to avoid overflo...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* external types supplied by the build */
extern seq_t a(idx_t n);

seq_t a(idx_t n)
{
    /* Static cache, grows on demand */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t next_to_compute = 0;   /* first index not yet computed */

    /* Handle trivial small indices */
    if (n < 2)
        return (seq_t)1;

    /* Ensure capacity for indices up to n */
    if (cache_cap <= n) {
        size_t new_cap = n + 1;
        if (cache == NULL) {
            cache = (seq_t *)calloc(new_cap, sizeof(seq_t));
            if (!cache) abort();
            cache_cap = new_cap;
            cache[0] = cache[1] = (seq_t)1;
            next_to_compute = 2;
        } else {
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cache_cap = new_cap;
            /* new slots are already zeroed by realloc (implementation
               dependent but safe for this use case).  */
            if (next_to_compute < 2) {
                cache[0] = cache[1] = (seq_t)1;
                next_to_compute = 2;
            }
        }
    }

    /* Compute missing entries */
    for (idx_t i = next_to_compute; i <= n; ++i) {
        seq_t res;
        /* If seq_t is <= 64‑bit, use a 128‑bit intermediate to avoid overflow */
        if (sizeof(seq_t) <= sizeof(unsigned long long)) {
            unsigned __int128 term1 = (unsigned __int128)(2 * i - 1) * (unsigned __int128)cache[i - 1];
            unsigned __int128 term2 = (unsigned __int128)63 * (unsigned __int128)(i - 1) * (unsigned __int128)cache[i - 2];
            unsigned __int128 sum  = term1 + term2;
            res = (seq_t)(sum / (unsigned __int128)i);
        } else {
            /* For larger types the normal arithmetic is exact */
            seq_t term1 = (seq_t)(2 * i - 1) * cache[i - 1];
            seq_t term2 = (seq_t)63 * (i - 1) * cache[i - 2];
            seq_t sum   = term1 + term2;
            res = sum / i;
        }
        cache[i] = res;
    }
    next_to_compute = n + 1;

    return cache[n];
}
