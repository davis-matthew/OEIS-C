/*
 * A021892  Decimal expansion of 1/888.
 * https://oeis.org/A021892
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(10 * (10^n mod 888) / 888)
 * Range:     u8 n <= 9999, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache of the remainders r[i] = 10^i mod 888     */
    /* r[0] = 1, r[i] = (r[i-1] * 10) % 888 */
    static uint16_t *rem = NULL;
    static size_t cache_cap = 0;   /* number of elements currently stored */

    /* initialise cache on first call */
    if (!rem) {
        rem = (uint16_t*)malloc(sizeof(uint16_t));
        if (!rem) return 0;             /* out of memory: return 0 */
        rem[0] = 1u % 888u;
        cache_cap = 1;
    }

    /* if requested n exceeds current cache, extend it */
    if ((idx_t)n >= (idx_t)cache_cap) {
        /* new required capacity */
        size_t new_cap = (size_t)(n + 1);
        /* reallocate the remainder array (keep existing data) */
        uint16_t *tmp = (uint16_t*)realloc(rem, new_cap * sizeof(uint16_t));
        if (!tmp) return 0;             /* allocation failed */
        rem = tmp;

        /* compute missing remainders */
        for (size_t i = cache_cap; i <= n; ++i) {
            rem[i] = (uint16_t)((rem[i - 1] * 10u) % 888u);
        }
        cache_cap = new_cap;
    }

    /* digit at position n is floor(10 * r[n] / 888) */
    uint16_t d = (uint16_t)((rem[n] * 10u) / 888u);
    return (seq_t)d;
}
