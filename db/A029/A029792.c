/*
 * A029792  Cubes k such that digits of cube root of k are not present in
 *          k^(2/3) or k.
 * https://oeis.org/A029792
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A029790(n)^3 where A029790(n) is the nth number m such
 *            that no decimal digit of m occurs in m^2 or in m^3. a(n) is the
 *            cube of that m. The function keeps a small cache of the values m
 *            so repeated calls are fast. It works for any build where seq_t
 *            is an unsigned integer type or an arbitrary...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 19, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   digits_mask
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/digits_mask.h"

seq_t a(idx_t n)
{
    /*  Using a modest cache size that covers the
        typical range of n (up to many thousands). */
    enum { CACHE_MAX = 20000 };
    static seq_t cache[CACHE_MAX];
    static size_t cache_len = 0;   /* number of cached m values */
    static seq_t next_candidate = 2;   /* smallest m that can work */

    seq_t last_m = 0;  /* holds the most recent valid m */

    /* Generate terms until we have at least n of them. */
    while (cache_len < n) {
        seq_t m = next_candidate;
        seq_t sq = m * m;
        seq_t cube = sq * m;
        unsigned int mask_m    = digits_mask(m);
        unsigned int mask_sq   = digits_mask(sq);
        unsigned int mask_cube = digits_mask(cube);

        if ( (mask_m & mask_sq) == 0 && (mask_m & mask_cube) == 0 ) {
            /* valid m found */
            last_m = m;
            if (cache_len < CACHE_MAX) {
                cache[cache_len++] = m;
            }
        }
        ++next_candidate;
    }

    /* Return the cube of the appropriate m */
    if (n <= CACHE_MAX) {
        seq_t m = cache[n - 1];
        return m * m * m;
    } else {
        return last_m * last_m * last_m;
    }
}
