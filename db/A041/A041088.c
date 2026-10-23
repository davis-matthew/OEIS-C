/*
 * A041088  Numerators of continued fraction convergents to sqrt(52).
 * https://oeis.org/A041088
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 35, u128 n <= 72,
 *            big any n
 * Helpers:   sqrt_floor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A041088: numerators of continued‑fraction convergents to sqrt(52).
 * Recurrence: a(n) = 1298*a(n–6) – a(n–12)  for n ≥ 12
 * Initial values (generated from the continued fraction of sqrt(52)):
 * a(0)=7, a(1)=29, a(2)=36, a(3)=101, a(4)=137, a(5)=649,
 * a(6)=9223, a(7)=37541, a(8)=46764, a(9)=131069,
 * a(10)=177833, a(11)=842401.
 *
 * The implementation computes the first 12 terms on first use by the
 * standard continued‑fraction algorithm for sqrt(52) and then extends
 * the array using the linear recurrence.  The values are stored in a
 * static dynamic array, so calls with arbitrary indices are handled
 * efficiently.
 */

#include <stdlib.h>
#include "../utils/sqrt_floor.h"

seq_t a(idx_t n)
{
    /* static cache for computed terms */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    const seq_t D = (seq_t)52;          /* the square‑free part */

    /* initialize cache on first call */
    if (cache == NULL) {
        /* we need the first 12 terms */
        const size_t base_len = 12;
        cache = (seq_t *)malloc(base_len * sizeof(seq_t));
        if (cache == NULL) abort();

        /* continued‑fraction data */
        seq_t m = 0;              /* m_0 */
        seq_t d = 1;              /* d_0 */
        seq_t a0 = sqrt_floor(D); /* a0 = floor(sqrt(52)) = 7 */
        seq_t a = a0;             /* current partial quotient */

        /* convergent recurrence */
        seq_t p_prev2 = 0;        /* p_{-2} */
        seq_t p_prev1 = 1;        /* p_{-1} */

        for (size_t i = 0; i < base_len; ++i) {
            /* numerator of the i‑th convergent */
            seq_t p = a * p_prev1 + p_prev2;
            cache[i] = p;

            /* shift for next iteration */
            p_prev2 = p_prev1;
            p_prev1 = p;

            /* compute next partial quotient (for i+1) */
            seq_t m_next = d * a - m;                /* m_{k+1} */
            seq_t d_next = (D - m_next * m_next) / d;/* d_{k+1} */
            seq_t a_next = (a0 + m_next) / d_next;   /* a_{k+1} */

            m = m_next;
            d = d_next;
            a = a_next;
        }
        cache_size = base_len;
    }

    /* if the requested index is already computed, just return it */
    if ((size_t)n < cache_size)
        return cache[(size_t)n];

    /* grow the cache up to n + 1 */
    size_t new_size = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (tmp == NULL) abort();
    cache = tmp;

    /* compute new terms using the recurrence */
    for (size_t i = cache_size; i <= n; ++i) {
        cache[i] = 1298 * cache[i - 6] - cache[i - 12];
    }
    cache_size = new_size;
    return cache[(size_t)n];
}
