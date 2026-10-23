/*
 * A041974  Numerators of continued fraction convergents to sqrt(510).
 * https://oeis.org/A041974
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 19, u64 n <= 41, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = Numerators of continued fraction convergents to sqrt(510) */
#include <stdlib.h>

/* The function returns the nth numerator of the continued–fraction
 * convergents to √510.  Internally it memoises the values so that
 * calls with arbitrary indices are answered in O(1) after the first
 * computation. */
seq_t a(idx_t n)
{
    /* constants for √510 */
    const seq_t D  = 510;   /* discriminant */
    const seq_t a0 = 22;    /* floor(√510) */

    /* static cache for memoisation */
    static seq_t *cache   = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* recurrence state (only valid after first initialisation) */
    static seq_t p_prev, p_prev2; /* previous two numerators            */
    static seq_t m, d, a_cur;     /* m_k, d_k, a_k in the standard CF algorithm */
    static int initialized = 0;   /* flag whether initial state is set     */

    /* generate terms until we have at least n */
    while (cache_len <= (size_t)n) {
        if (!initialized) {
            /* first term (k = 0) */
            if (cache_cap == 0) {
                cache_cap = 16;
                cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
            }
            cache[0] = a0;               /* p0 */
            cache_len = 1;

            /* initialise recurrence variables for the next term */
            p_prev  = a0;                /* p0   */
            p_prev2 = 1;                 /* p_{-1} */
            m  = 0;
            d  = 1;
            a_cur = a0;
            initialized = 1;
        } else {
            /* compute next convergent numerator */
            seq_t m_next = d * a_cur - m;
            seq_t d_next = (D - m_next * m_next) / d;
            seq_t a_next = (a0 + m_next) / d_next;
            seq_t p_next = a_next * p_prev + p_prev2;

            /* enlarge cache if needed */
            if (cache_len >= cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = p_next;

            /* shift state for the following iteration */
            p_prev2 = p_prev;
            p_prev  = p_next;
            m  = m_next;
            d  = d_next;
            a_cur = a_next;
        }
    }
    return cache[(size_t)n];
}
