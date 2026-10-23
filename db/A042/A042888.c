/*
 * A042888  Numerators of continued fraction convergents to sqrt(976).
 * https://oeis.org/A042888
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 3532638098*a(n-18) - a(n-36) (for n > 35) The first 36
 *            terms are generated from the convergents of √976
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 17, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* ----------------------------------------------------------
       Persistent cache of computed terms.
       cache_len  – number of terms computed so far (next free slot)
       cache_cap  – allocated capacity of the cache array
       cache      – pointer to the array of terms
    ---------------------------------------------------------- */
    static seq_t *cache     = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* ----------------------------------------------------------
       State for generating the first 36 terms using the
       continued‑fraction algorithm for sqrt(976).
    ---------------------------------------------------------- */
    static uint64_t m      = 0, d = 1, a_cur = 0;          /* current m,d,a */
    static seq_t  p_pp = 0;                              /* p_{k-2} */
    static seq_t  p_p  = 1;                              /* p_{k-1} */
    static int    cf_initialized = 0;                    /* flag */

    /* sqrt(976) = 31.xxx … */
    const uint64_t D = 976;
    const uint64_t a0 = 31;                              /* floor(sqrt(D)) */

    /* ------------------------ initialization ------------------------ */
    if (!cf_initialized) {
        m      = 0;
        d      = 1;
        a_cur  = a0;
        p_pp   = 0;
        p_p    = 1;
        cf_initialized = 1;
    }

    /* ---------------------- compute up to n ------------------------ */
    while (cache_len <= (size_t)n) {
        seq_t term;

        if (cache_len < 36) {       /* first 36 terms – use CF */
            term = ((seq_t)a_cur) * p_p + p_pp;

            /* update cache before advancing CF state */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap == 0 ? 16 : cache_cap << 1;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return 0; /* out of memory – return 0 as fallback */
                cache     = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = term;

            /* advance continued‑fraction state for next term */
            uint64_t m_next = d * a_cur - m;
            uint64_t d_next = (D - m_next * m_next) / d;
            uint64_t a_next = (a0 + m_next) / d_next;
            m = m_next;
            d = d_next;
            a_cur = a_next;

            /* shift p array for next convergent */
            seq_t old_pp = p_pp;
            p_pp = p_p;
            p_p  = term;
        } else { /* cache_len >= 36 – use the 36‑term recurrence */
            term = ((seq_t)3532638098ULL) * cache[cache_len - 18] - cache[cache_len - 36];

            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap == 0 ? 16 : cache_cap << 1;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return 0;
                cache     = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = term;
        }
    }

    return cache[(size_t)n];
}
