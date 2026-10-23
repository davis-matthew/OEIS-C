/*
 * A145010  a(n) = area of Pythagorean triangle with hypotenuse p, where p =
 *          A002144(n) = n-th prime == 1 (mod 4).
 * https://oeis.org/A145010
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = area of the primitive Pythagorean triangle with
 *            hypotenuse the nth prime ≡ 1 (mod 4). The area is
 *            A002365(n)*A002366(n)/2 = x*y*(x² − y²) where p = A002144(n) =
 *            x² + y²
 * Range:     u8 n <= 6, u16 n <= 46, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    /* Static cache for computed values */
    static seq_t *cache      = NULL;   /* array of a(k) values */
    static size_t cache_cap  = 0;      /* allocated capacity */
    static idx_t  cache_size = 0;      /* number of values computed so far */

    /* State for generating primes congruent to 1 (mod 4) */
    static uint64_t next_candidate = 5;   /* next odd number to test */

    if (n == 0) return 0;  /* Undefined for n=0, return 0 */

    /* Expand cache capacity if necessary */
    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 128;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_ptr) exit(1);
        cache     = new_ptr;
        cache_cap = new_cap;
    }

    /* Return value from cache if already computed */
    if (n <= cache_size) {
        return cache[n-1];
    }

    /* Compute terms from cache_size+1 up to n */
    for (idx_t idx = cache_size + 1; idx <= n; ++idx) {
        /* Find the next prime ≡ 1 (mod 4) */
        while (1) {
            if ((next_candidate & 3ULL) == 1 && is_prime((idx_t)next_candidate)) {
                break;
            }
            next_candidate += 2;  /* skip even numbers */
        }
        uint64_t p = next_candidate;

        /* Find representation p = x² + y² with x > y */
        seq_t sq_p = (seq_t)p;
        seq_t x = floor_sqrt(sq_p);
        seq_t y;
        while (1) {
            seq_t rem = sq_p - x * x;
            seq_t y_candidate = floor_sqrt(rem);
            if (y_candidate * y_candidate == rem) {
                y = y_candidate;
                break;
            }
            x -= 1;
        }

        /* Compute the area: x * y * (x² − y²) */
        seq_t area = (x * y) * (x * x - y * y);

        /* Store in cache */
        cache[idx - 1] = area;

        /* Prepare for next prime search */
        next_candidate = p + 2;  /* next odd number after current prime */
    }

    cache_size = n;
    return cache[n-1];
}
