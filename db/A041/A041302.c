/*
 * A041302  Numerators of continued fraction convergents to sqrt(164).
 * https://oeis.org/A041302
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, cofr, easy
 * Formula:   a(n) = 4098*a(n-6) - a(n-12) a(0)=12, a(1)=13, a(2)=64,
 *            a(3)=397, a(4)=1652, a(5)=2049, a(6)=50828, a(7)=52877,
 *            a(8)=262336, a(9)=1626893, a(10)=6769908, a(11)=8396801
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 14, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Continuous‑fraction convergents of sqrt(164) generate the first 12
   terms.  The recurrence a(n)=4098*a(n-6)-a(n-12) holds for n≥12. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;          /* next index to compute */

    size_t idx = (size_t)n;                /* convert index for array */

    /* Allocate minimal space (useless but safe). */
    if (cache == NULL) {
        cache_cap = idx + 1;
        if (cache_cap < 12) cache_cap = 12;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return 0;       /* allocation failure */
        cache_len = 0;
    }

    /* Grow cache if needed. */
    if (idx + 1 > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < idx + 1) new_cap = idx + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;         /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* --- compute first 12 terms via continued fraction --- */
    if (cache_len < 12) {
        /* Continued‑fraction variables for sqrt(164). */
        unsigned long long m = 0;                         /* m_0 */
        unsigned long long d = 1;                         /* d_0 */
        unsigned long long a_k = 12;                      /* a_0 = floor(sqrt(164)) */

        /* Convergents:  p_-2 = 0, p_-1 = 1 */
        seq_t p_prev2 = 0;
        seq_t p_prev1 = 1;

        for (size_t k = 0; k < 12; ++k) {
            /* p_k = a_k * p_{k-1} + p_{k-2} */
            seq_t p_curr = (seq_t)((seq_t)a_k * p_prev1 + p_prev2);
            cache[k] = p_curr;

            /* Prepare next continued‑fraction coefficient. */
            if (k < 11) {      /* need next a_k only for the next iteration */
                unsigned long long m_next = d * a_k - m;
                unsigned long long d_next = (164ULL - m_next * m_next) / d;
                unsigned long long a_next = (12ULL + m_next) / d_next;

                m = m_next;
                d = d_next;
                a_k = a_next;
            }

            /* Shift convergent values. */
            p_prev2 = p_prev1;
            p_prev1 = p_curr;
        }
        cache_len = 12;
    }

    /* --- use recurrence for n ≥ 12 --- */
    while (cache_len <= idx) {
        /* a(i) = 4098*a(i-6) - a(i-12) */
        seq_t term = cache[cache_len - 6];
        /* Multiply by 4098 = 2^12 + 2^1  via shifts to avoid overflow on
           unsigned small types while still working on big types. */
        term <<= 12;                      /* term *= 4096 */
        term += cache[cache_len - 6] << 1;/* term += 2 * a(i-6) */
        term -= cache[cache_len - 12];     /* term -= a(i-12) */

        cache[cache_len] = term;
        ++cache_len;
    }

    return cache[idx];
}
