/*
 * A040847  Continued fraction for sqrt(877).
 * https://oeis.org/A040847
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   simple continued fraction coefficients of √877 a(0) = ⌊√877⌋ =
 *            29, a(n) computed by the standard quadratic‑surd algorithm
 * Range:     u8 n <= 1, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External types supplied by the build:
   typedef <some unsigned> seq_t;   // sequence element type
   typedef uint64_t idx_t;          // index type
*/

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static size_t cache_len  = 0;   /* number of stored terms */

    /* quadratic surd parameters for √877 */
    static const seq_t D   = 877;   /* constant D */
    static const seq_t a0  = 29;    /* floor(√D) */
    static seq_t m          = 0;    /* current m term  */
    static seq_t d          = 1;    /* current d term  */

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap  = 64;                      /* start with a small size */
        cache      = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache_len  = 1;
        cache[0] = a0;                        /* a(0) */
        m = 0; d = 1;
    }

    /* grow cache if necessary */
    while ((idx_t)cache_len <= n) {
        /* compute a_next from a_prev, m, d */
        seq_t a_prev   = cache[cache_len - 1];
        seq_t m_next   = d * a_prev - m;
        seq_t d_next   = (D - m_next * m_next) / d;
        seq_t a_next   = (a0 + m_next) / d_next;

        /* store the new term */
        if (cache_len >= cache_cap) {
            cache_cap *= 2;
            cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
        }
        cache[cache_len] = a_next;
        cache_len++;

        /* update state for next iteration */
        m = m_next;
        d = d_next;
    }

    return cache[n];
}
