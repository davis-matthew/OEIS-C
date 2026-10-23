/*
 * A042894  Numerators of continued fraction convergents to sqrt(979).
 * https://oeis.org/A042894
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = numerator of the nth convergent of the continued fraction
 *            expansion of sqrt(979) (offset 0)
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 14, u64 n <= 30, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* External linkage as required. */
seq_t a(idx_t n)
{
    /* Constants for sqrt(979) */
    const uint64_t D = 979ULL;      /* D = 979 */
    const uint64_t a0 = 31ULL;      /* floor(sqrt(979)) = 31 */

    /* Static cache for results. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* Cache state for future extensions. */
    static uint64_t m = 0;
    static uint64_t d = 1;
    static uint64_t cur_a = 31;     /* a0 initial */
    static seq_t prev2 = 0;         /* p_{-2} */
    static seq_t prev1 = 1;         /* p_{-1} */

    /* Allocate initial cache if first call. */
    if (cache == NULL) {
        cache_cap = 32;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* Extend cache up to requested index n. */
    while (cache_len <= (size_t)n) {
        /* Compute next numerator: p = cur_a * prev1 + prev2 */
        seq_t cur = (seq_t)cur_a * prev1 + prev2;

        /* Grow cache if necessary. */
        if (cache_len >= cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) {
                /* Allocation failed – abort gracefully. */
                exit(1);
            }
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Store computed term. */
        cache[cache_len++] = cur;

        /* Prepare state for next iteration (continued fraction). */
        uint64_t m_next = d * cur_a - m;
        uint64_t d_next = (D - m_next * m_next) / d;
        uint64_t a_next = (a0 + m_next) / d_next;

        /* Update for next step. */
        m = m_next;
        d = d_next;
        cur_a = a_next;

        /* Shift previous numerators. */
        prev2 = prev1;
        prev1 = cur;
    }

    /* Return cached result. */
    return cache[(size_t)n];
}
