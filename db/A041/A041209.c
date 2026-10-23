/*
 * A041209  Denominators of continued fraction convergents to sqrt(115).
 * https://oeis.org/A041209
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 2252*a(n-10) - a(n-20); a(0)=1, a(1)=1, a(2)=3, a(3)=4,
 *            a(4)=7, a(5)=11, a(6)=18, a(7)=29, a(8)=76, a(9)=105,
 *            a(10)=2176, a(11)=2281, a(12)=6738, a(13)=9019, a(14)=15757,
 *            a(15)=24776, a(16)=40533, a(17)=65309, a(18)=171151,
 *            a(19)=236460
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 29, u64 n <= 59, u128 n <= 117,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Base terms for indices 0..19. */
    static const uint64_t base[20] = {
        1ULL, 1ULL, 3ULL, 4ULL, 7ULL, 11ULL,
        18ULL, 29ULL, 76ULL, 105ULL,
        2176ULL, 2281ULL, 6738ULL, 9019ULL,
        15757ULL, 24776ULL, 40533ULL, 65309ULL,
        171151ULL, 236460ULL
    };

    /* Static cache for computed values. */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of valid entries 0..cache_size-1 */
    static size_t cache_cap  = 0;   /* allocated capacity */

    /* For small indices, return the constant base term. */
    if (n < 20) {
        return (seq_t)base[(size_t)n];
    }

    /* Initialise cache on first use. */
    if (cache == NULL) {
        cache_cap = 20;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;  /* unlikely – allocation failure */
    }

    /* Expand the cache if needed to cover index n. */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;  /* unlikely – allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n inclusively. */
    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        if (i < 20) {
            cache[i] = (seq_t)base[i];
        } else {
            cache[i] = cache[i-10] * (seq_t)2252ULL - cache[i-20];
        }
    }
    cache_size = (size_t)n + 1;

    return cache[(size_t)n];
}
