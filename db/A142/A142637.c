/*
 * A142637  Primes congruent to 51 mod 55.
 * https://oeis.org/A142637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 164, u32 n <= 165, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the nth prime congruent to 51 mod 55 */

#include "../utils/is_prime_big.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; n >= 1 for valid calls */

    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t found = 0;
    static idx_t current_k = 4;    /* start from 55*4+51 = 271 */

    const seq_t MOD = 55;
    const seq_t OFF = 51;

    if (n <= found)                /*  earlier computed values */
        return cache[n - 1];

    /* --- enlarge cache if needed --------------------------------------- */
    if (cache == NULL) {
        cache_cap = n > 16 ? n : 16;
        cache = seq_realloc(NULL, 0, cache_cap);
    } else if (n > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < n) new_cap = n;
        seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* --- compute missing primes ---------------------------------------- */
    while (found < n) {
        seq_t candidate = (seq_t)current_k * MOD + OFF;
        if (is_prime_big(candidate)) {
            cache[found] = candidate;
            found++;
        }
        current_k++;
    }

    return cache[n - 1];
}
