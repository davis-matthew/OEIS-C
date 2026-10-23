/*
 * A040901  Continued fraction for sqrt(932).
 * https://oeis.org/A040901
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction for sqrt(932)
   a0 = floor(sqrt(932)) = 30
   For k >= 1:
      m_k = d_{k-1} * a_{k-1} - m_{k-1}
      d_k = (932 - m_k*m_k) / d_{k-1}
      a_k = (a0 + m_k) / d_k   (integer division)
*/
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache for computed terms and recurrence state */
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;

    static const unsigned long long A0 = 30ULL; /* floor(sqrt(932)) */
    static unsigned long long m = 0ULL;       /* previous m   */
    static unsigned long long d = 1ULL;       /* previous d   */
    static unsigned long long a_prev = A0;   /* previous a   */
    static int initialized = 0;

    if (!initialized) {
        /* initialise cache with a0 */
        cache_cap = 8;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) return (seq_t)0;
        cache[0] = (seq_t)A0;
        cache_len = 1;
        initialized = 1;
    }

    /* extend cache up to index n */
    while (cache_len <= (size_t)n) {
        if (cache_len == cache_cap) {
            cache_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
            if (!tmp) return (seq_t)0;
            cache = tmp;
        }

        /* compute next term a_k using recurrence */
        unsigned long long m_next = d * a_prev - m;
        unsigned long long d_next = (932ULL - m_next * m_next) / d;
        unsigned long long a_next = (A0 + m_next) / d_next;

        /* update state for subsequent steps */
        m = m_next;
        d = d_next;
        a_prev = a_next;

        /* store the term */
        cache[cache_len] = (seq_t)a_next;
        cache_len++;
    }

    return cache[(size_t)n];
}
