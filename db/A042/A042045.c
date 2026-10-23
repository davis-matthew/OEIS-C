/*
 * A042045  Denominators of continued fraction convergents to sqrt(546).
 * https://oeis.org/A042045
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = Denominators of continued fraction convergents to
 *            sqrt(546). Sequence satisfies recurrence
 *            a(n)=1402*a(n-6)-a(n-12) for n>11. Initial terms: a(0)=1,
 *            a(1)=2, a(2)=3, a(3)=8, a(4)=11, a(5)=30, a(6)=1391, a(7)=2812,
 *            a(8)=4203, a(9)=11218, a(10)=15421, a(11)=42060
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 18, u64 n <= 37, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* Static cache of previously computed terms */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static size_t cache_size = 0;

    /* State for generating continued‑fraction coefficients of sqrt(546) */
    static uint64_t m_cur = 0, d_cur = 1, a_cur = 23; /* a_cur holds the last used a_k */
    static const uint64_t D     = 546;
    static const uint64_t a0_val = 23;                 /* a0 = floor(sqrt(546)) */
    static seq_t q_prev1 = 1; /* q_{k-1}, initialized to q0 */
    static seq_t q_prev2 = 0; /* q_{k-2}, initialized to q-1 */

    /* Initialisation on first call */
    if (!cache) {
        cache_cap  = 1;
        cache_size = 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;                /* Allocation failed, return 0 */
        cache[0]   = (seq_t)1;               /* a(0) = 1 */
    }

    /* Extend cache if needed */
    if (n >= cache_size) {
        /* Grow cache to at least n+1 elements */
        size_t needed = n + 1;
        if (needed > cache_cap) {
            size_t new_cap = cache_cap * 2;
            if (new_cap < needed) new_cap = needed;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;            /* Allocation failed, return 0 */
            cache = tmp;
            cache_cap = new_cap;
        }
        /* Compute new terms up to n */
        for (size_t k = cache_size; k <= (size_t)n; ++k) {
            /* Generate next continued‑fraction coefficient a_k */
            uint64_t m_next = d_cur * a_cur - m_cur;
            uint64_t d_next = (D - m_next * m_next) / d_cur;
            uint64_t a_next = (a0_val + m_next) / d_next;

            seq_t a_next_seq = (seq_t)a_next;
            seq_t q_curr = a_next_seq * q_prev1 + q_prev2;

            cache[k] = q_curr;

            /* Prepare for next iteration */
            q_prev2 = q_prev1;
            q_prev1 = q_curr;
            m_cur   = m_next;
            d_cur   = d_next;
            a_cur   = a_next;
        }
        cache_size = n + 1;
    }
    return cache[n];
}
