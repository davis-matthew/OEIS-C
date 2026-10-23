/*
 * A119570  Primes p such that p^2 - p - 1 is not prime.
 * https://oeis.org/A119570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th prime p such that p^2 - p - 1 is NOT prime
 *            (offset = 1)
 * Range:     u8 n <= 2, u16 n <= 33, u32 n <= 36, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   is_prime_big, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* Return the n-th element of sequence A119570. */
seq_t a(idx_t n)
{
    if (n == 0)                     /* behaviour below offset is irrelevant */
        return (seq_t)0;

    /* Static cache to allow out‑of‑order and repeated calls. */
    static seq_t *cache     = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t  last_checked = 1;   /* start just before the first prime */

    /* Extend the cache until it contains at least n elements. */
    while (cache_len < n) {
        /* Find the next prime number. */
        do {
            ++last_checked;
        } while (!is_prime_big((seq_t)last_checked));

        seq_t p = (seq_t)last_checked;
        seq_t target = p * p - p - (seq_t)1;  /* p^2 - p - 1 */

        /* Store p if the target is composite. */
        if (!is_prime_big(target)) {
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 128;
                if (cache == NULL) {
                    cache = seq_alloc(new_cap);
                } else {
                    cache = seq_realloc(cache, cache_cap, new_cap);
                }
                cache_cap = new_cap;
            }
            cache[cache_len++] = p;
        }
    }

    return cache[n - 1];   /* sequence is 1‑indexed */
}
