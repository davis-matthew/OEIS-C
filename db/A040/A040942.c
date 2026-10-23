/*
 * A040942  Continued fraction for sqrt(974).
 * https://oeis.org/A040942
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) = the n-th partial quotient in the simple continued
 *            fraction expansion of sqrt(974) (using the standard recurrence)
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static bool          initialized = false;
    static seq_t        *cache      = NULL;
    static idx_t        len         = 0;
    static idx_t        cap         = 0;
    static seq_t        a_prev;
    static idx_t        m, d;
    static const idx_t  a0 = 31;          /* floor(sqrt(974)) */

    if (!initialized) {
        a_prev = (seq_t)a0;               /* a(0) */
        m      = 0;
        d      = 1;

        cap   = 32;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;             /* allocation failure */

        cache[0] = (seq_t)a0;
        len      = 1;
        initialized = true;
    }

    /* Extend cache until it contains at least n+1 terms */
    while (len <= n) {
        /* compute the next term */
        idx_t  a_prev_i = (idx_t)a_prev;        /* cast for multiplication */
        idx_t  m_next   = d * a_prev_i - m;
        idx_t  diff     = (idx_t)974 - m_next * m_next;
        idx_t  d_next   = diff / d;
        idx_t  a_next_i = (a0 + m_next) / d_next;
        seq_t  next_a   = (seq_t)a_next_i;

        /* grow cache capacity if necessary */
        if (len == cap) {
            size_t new_cap = cap * 2;
            seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (new_cache) {
                cache = new_cache;
                cap   = new_cap;
            }            /* if realloc fails, keep old cache (unlikely) */
        }

        cache[len++] = next_a;
        a_prev = next_a;
        m      = m_next;
        d      = d_next;
    }

    return cache[n];
}
