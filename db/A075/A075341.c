/*
 * A075341  a(1) = 1, a(2n) is the smallest composite number == 1 mod
 *          (a(2n-1)) and a(2n+1) is the smallest prime == 1 (mod a(2n)).
 * https://oeis.org/A075341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; a(2n) = the smallest composite ≡ 1 (mod a(2n − 1));
 *            a(2n+1) = the smallest prime ≡ 1 (mod a(2n))
 * Range:     u8 n <= 18, u16 n <= 28, u32 n <= 28, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

/* Find the smallest composite ≡ 1 (mod mod). */
static seq_t find_composite(seq_t mod)
{
    seq_t k = 1;
    for (;; ++k) {
        seq_t cand = mod * k + 1;
        if (!is_prime_big(cand))
            return cand;
    }
}

/* Find the smallest prime ≡ 1 (mod mod). */
static seq_t find_prime(seq_t mod)
{
    seq_t k = 1;
    for (;; ++k) {
        seq_t cand = mod * k + 1;
        if (is_prime_big(cand))
            return cand;
    }
}

/* Returns the n‑th term of A075341 (1‑indexed). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* out of range */

    /* Static cache for computed terms. */
    static seq_t *cache = NULL;
    static idx_t max_computed = 0;
    static size_t capacity = 0;

    if (n <= max_computed)
        return cache[n];

    /* Expand cache if necessary. */
    if (capacity < (size_t)(n + 1)) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache)
            return (seq_t)0;        /* allocation failure */

        /* Copy existing data. */
        for (size_t i = 0; i < capacity; ++i)
            new_cache[i] = cache[i];

        if (cache)
            free(cache);

        cache = new_cache;
        capacity = new_cap;
    }

    /* Compute terms up to n. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i == 1) {
            cache[1] = (seq_t)1;
        } else if ((i & 1) == 0) {        /* even index */
            cache[i] = find_composite(cache[i - 1]);
        } else {                          /* odd index > 1 */
            cache[i] = find_prime(cache[i - 1]);
        }
    }

    max_computed = n;
    return cache[n];
}
