/*
 * A129672  Right truncatable primes in base 6 (written in decimal form).
 * https://oeis.org/A129672
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, base
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 35, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   is_prime_small, seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th right truncatable prime in base 6 (written in decimal)
 * The algorithm repeatedly generates decimal primes and checks that
 * dividing them by 6 yields primes until the value becomes zero.
 * The first 36 such primes exist, so the function caches them
 * and returns the n‑th element (1‑based). For n > 36, 0 is returned. */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/is_prime_small.h"
#include "../utils/seq_alloc_new_array.h"

static seq_t *cache = NULL;          /* array holding the found primes */
static size_t cache_size = 0;        /* number of cached primes (≤36) */
static bool initialized = false;    /* did we fill the cache yet? */

/* Helper that populates the cache with all 36 right‑truncatable primes. */
static void build_cache(void)
{
    const size_t NEED = 36;
    cache = seq_alloc_new_array(NEED);
    size_t count = 0;

    for (seq_t p = 2; count < NEED; ++p) {
        if (!is_prime_small(p))
            continue;

        bool ok = true;
        seq_t temp = p;
        while (temp > 0) {
            if (!is_prime_small(temp)) { ok = false; break; }
            temp /= 6;
        }
        if (ok) {
            cache[count++] = p;
        }
    }
    cache_size = NEED;
    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized)
        build_cache();

    if (n == 0)
        return 0;                       /* undefined for n < 1 */
    idx_t pos = n - 1;                   /* offset 1 */
    if (pos >= cache_size)
        return 0;                       /* beyond known terms; sequence ends */
    return cache[pos];
}
