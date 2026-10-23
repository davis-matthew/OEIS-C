/*
 * A131963  Expansion of f(x, x^2) * f(x^4, x^12) in powers of x where f(, )
 *          is Ramanujan's general theta function.
 * https://oeis.org/A131963
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = coefficient of x^n in f(x, x^2) * f(x^4, x^12), * where
 *            f(a,b) = Σ_{k∈ℤ} a^{k(k+1)/2} b^{k(k-1)/2}. * The exponents of
 *            the first series are e₁(k)=k(3k-1)/2 for all * integer k. The
 *            exponents of the second series are e₂(k)=4k(2k-1) * for all
 *            integer k. a(n) is the number of ordered pairs...
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   compute_a_single, push_exponent
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/compute_a_single.h"
#include "../utils/push_exponent.h"

/* ---------- main sequence function with memoisation ---------- */
seq_t a(idx_t n)
{
    /* static cache of already computed values */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    if (n < cache_size) {
        return cache[n];
    }

    /* extend cache to include indices up to n */
    size_t new_size = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;

    for (size_t i = cache_size; i < new_size; ++i) {
        cache[i] = compute_a_single((idx_t)i);
    }
    cache_size = new_size;
    return cache[n];
}
