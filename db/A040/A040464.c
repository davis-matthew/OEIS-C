/*
 * A040464  Continued fraction for sqrt(487).
 * https://oeis.org/A040464
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   a(n) = n-th coefficient of the simple continued fraction of √487
 * Range:     u8 n <= 1, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_alloc = 0;
    static seq_t m = 0;
    static seq_t d = 1;
    static seq_t a_prev = 22;          /* a0 */
    const seq_t D   = (seq_t)487;      /* the radicand */
    const seq_t a0  = (seq_t)22;       /* floor(√487) */

    /* Ensure cache holds a[0] */
    if (cache_len == 0) {
        if (!cache) {
            cache = (seq_t *)malloc(sizeof(seq_t));
            cache_alloc = 1;
        }
        cache[0] = a_prev;
        cache_len = 1;
    }

    /* Grow cache if necessary */
    if ((size_t)n >= cache_len) {
        size_t needed = (size_t)n + 1;
        if (needed > cache_alloc) {
            size_t new_alloc = cache_alloc ? cache_alloc * 2 : 4;
            while (new_alloc < needed)
                new_alloc <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_alloc * sizeof(seq_t));
            cache = tmp;
            cache_alloc = new_alloc;
        }
        /* Compute new terms */
        for (size_t idx = cache_len; idx < needed; ++idx) {
            seq_t m_next = d * a_prev - m;
            seq_t d_next = (D - m_next * m_next) / d;
            seq_t a_next = (a0 + m_next) / d_next;
            cache[idx] = a_next;
            m = m_next;
            d = d_next;
            a_prev = a_next;
        }
        cache_len = needed;
    }
    return cache[(size_t)n];
}
