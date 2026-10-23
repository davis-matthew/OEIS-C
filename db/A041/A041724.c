/*
 * A041724  Numerators of continued fraction convergents to sqrt(382).
 * https://oeis.org/A041724
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 329996879998*a(n-24) - a(n-48) for n > 47. Initial values
 *            (from the continued fraction of √382): a(0)=19, a(1)=20,
 *            a(2)=39, a(3)=215, a(4)=2619, a(5)=2834, a(6)=16789, a(7)=19623,
 *            a(8)=36412, a(9)=92447, a(10)=313753, a(11)=406200,
 *            a(12)=7625353, a(13)=8031553,
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 19, u64 n <= 38, u128 n <= 77,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;
    const seq_t C = (seq_t)329996879998ull;

    /* compute and cache the first 48 convergents of √382 */
    if (cache == NULL) {
        cache_cap = 48;
        cache = seq_alloc(cache_cap);

        const uint64_t D = 382;      /* square‑free part of the radicand */
        const uint64_t a0 = 19;      /* floor(√382) */
        uint64_t m = 0, d = 1;
        uint64_t prev_a = a0;
        uint64_t coeff[48];
        coeff[0] = a0;

        /* continued‑fraction coefficients a1 … a47 */
        for (size_t i = 1; i < 48; ++i) {
            m = d * prev_a - m;
            d = (D - m * m) / d;
            prev_a = (a0 + m) / d;
            coeff[i] = prev_a;
        }

        /* compute convergent numerators p0 … p47 */
        seq_t p_m2 = 0;  /* p_{-2} */
        seq_t p_m1 = 1;  /* p_{-1} */
        for (size_t i = 0; i < 48; ++i) {
            seq_t p = coeff[i] * p_m1 + p_m2;
            cache[i] = p;
            p_m2 = p_m1;
            p_m1 = p;
        }
        cache_len = 48;
    } else if ((size_t)n + 1 > cache_cap) {  /* grow cache if needed */
        size_t new_cap = cache_cap;
        while ((size_t)n + 1 > new_cap) new_cap <<= 1;
        cache = seq_realloc(cache, cache_cap, new_cap);
        cache_cap = new_cap;
    }

    /* extend cache with the recurrence for n ≥ 48 */
    if ((size_t)n >= cache_len) {
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            cache[i] = C * cache[i - 24] - cache[i - 48];
        }
        cache_len = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
