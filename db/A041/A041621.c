/*
 * A041621  Denominators of continued fraction convergents to sqrt(329).
 * https://oeis.org/A041621
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of the nth convergent to sqrt(329).
 *            Recurrence: a(n) = 4752830*a(n-12) - a(n-24) for n > 23
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    size_t target = (size_t)n;

    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_size = 0;
    static uint64_t cf_m = 0, cf_d = 1, cf_a = 18;
    static const uint64_t D   = 329;
    static const uint64_t a0  = 18;        /* floor(sqrt(329)) */
    static int init = 0;

    if (!init) {
        cache_cap = target + 1 > 256 ? target + 1 : 256;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        init = 1;
        cache_size = 0;
        cf_m = 0;
        cf_d = 1;
        cf_a = a0;
    }

    if (target + 1 > cache_cap) {
        size_t new_cap = target + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;
        cache = tmp;
        cache_cap = new_cap;
    }

    for (size_t i = cache_size; i <= target; ++i) {
        seq_t q;
        if (i == 0) {
            q = 1;
        } else {
            seq_t prev1 = cache[i-1];
            seq_t prev2 = (i >= 2) ? cache[i-2] : (seq_t)0;
            q = (seq_t)cf_a * prev1 + prev2;
        }
        cache[i] = q;

        /* Compute next partial quotient for continued fraction. */
        uint64_t m_next = cf_d * cf_a - cf_m;
        uint64_t d_next = (D - m_next * m_next) / cf_d;
        uint64_t a_next = (a0 + m_next) / d_next;

        cf_m = m_next;
        cf_d = d_next;
        cf_a = a_next;
    }

    cache_size = target + 1;
    return cache[target];
}
