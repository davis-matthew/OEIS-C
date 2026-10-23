/*
 * A167293  Long legs of Pythagorean triangles that are not divisible by any
 *          other long leg of a Pythagorean triangle.
 * https://oeis.org/A167293
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   append_candidate
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A167293: Long legs of Pythagorean triangles that are not divisible by any
   other long leg.  We generate all primitive long legs up to a growing
   upper bound and sieve out multiples, keeping only the “primitive”
   ones.  The function is memoised so that repeated calls with
   arbitrary order of indices are fast. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/append_candidate.h"

static inline idx_t gcd_uint64(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* static cache of generated values */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t current_U = 0;

    if (n == 0) return 0; /* undefined for n = 0, but we return 0 */

    /* regenerate cache if needed */
    while (cache_len < n) {
        /* increase upper bound */
        idx_t U = current_U == 0 ? 200000ULL : current_U * 2ULL;
        /* allocate boolean array for primality marks */
        uint8_t *is_long_leg = (uint8_t *)calloc((size_t)(U + 1), sizeof(uint8_t));
        if (!is_long_leg) return 0; /* allocation failure */

        /* find suitable m_max: (m+1)^2 <= 2*U */
        idx_t m_max = 1;
        while (((m_max + 1) * (m_max + 1)) <= U * 2) ++m_max;

        /* generate primitive long legs */
        for (idx_t m = 2; m <= m_max; ++m) {
            for (idx_t n_ = 1; n_ < m; ++n_) {
                if (((m - n_) & 1) == 0) continue;          /* opposite parity */
                if (gcd_uint64(m, n_) != 1) continue;       /* coprime */
                idx_t leg1 = m * m - n_ * n_;
                idx_t leg2 = 2 * m * n_;
                idx_t l = (leg1 > leg2) ? leg1 : leg2;      /* long leg */
                if (l <= U) is_long_leg[l] = 1;
            }
        }

        /* sieve out multiples of earlier long legs */
        for (idx_t l = 4; l <= U; ++l) {
            if (is_long_leg[l]) {
                append_candidate(&cache, &cache_len, &cache_cap, (seq_t)l);
                for (idx_t mult = l + l; mult <= U; mult += l) {
                    is_long_leg[mult] = 0;
                }
            }
        }

        free(is_long_leg);
        current_U = U;
    }

    return cache[n - 1];
}
