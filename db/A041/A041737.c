/*
 * A041737  Denominators of continued fraction convergents to sqrt(388).
 * https://oeis.org/A041737
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of the n-th convergent to sqrt(388)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 34, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache for computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Continued‑fraction state */
    static uint64_t m_cur = 0;          /* current m_k   */
    static uint64_t d_cur = 1;          /* current d_k   */
    static uint64_t a_cur = 0;          /* current a_k   */
    static const uint64_t a0 = 19ULL;   /* floor(sqrt(388)) */

    /* Initialize on first use */
    if (cache_len == 0) {
        /* Allocate initial cache */
        cache_cap = 4;
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0; /* unexpected failure */

        /* q_0 = 1 */
        cache[0] = (seq_t)1;
        cache_len = 1;

        /* Compute first partial quotient a_1 */
        uint64_t m_next = a_cur == 0 ? a0 : d_cur * a_cur - m_cur; /* m_1 = 19 */
        uint64_t d_next = (388ULL - m_next * m_next) / d_cur;      /* d_1 = 27 */
        a_cur = (a0 + m_next) / d_next;                           /* a_1 = 1 */
        m_cur = m_next;
        d_cur = d_next;
    }

    /* Extend cache until required index is present */
    while (cache_len <= n) {
        /* Ensure capacity */
        if (cache_len >= cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL) return (seq_t)0; /* unexpected failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Compute next denominator q_k */
        seq_t q_prev1 = cache[cache_len - 1];
        seq_t q_prev2 = (cache_len >= 2) ? cache[cache_len - 2] : (seq_t)0;
        seq_t q_new = (seq_t)a_cur * q_prev1 + q_prev2;

        /* Store new term */
        cache[cache_len++] = q_new;

        /* Advance continued‑fraction state for next partial quotient */
        uint64_t m_next = d_cur * a_cur - m_cur;
        uint64_t d_next = (388ULL - m_next * m_next) / d_cur;
        uint64_t a_next = (a0 + m_next) / d_next;

        m_cur = m_next;
        d_cur = d_next;
        a_cur = a_next;
    }

    return cache[n];
}
