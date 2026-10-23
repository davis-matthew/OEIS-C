/*
 * A042547  Denominators of continued fraction convergents to sqrt(802).
 * https://oeis.org/A042547
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of the n-th convergent to sqrt(802) *
 *            Recurrence: a(0)=1, a(1)=3, * a(n) = c_n * a(n-1) + a(n-2)
 *            (n>=2) * where c_n are the continued‑fraction partial quotients
 *            of sqrt(802) * (starting with the first after the integer part)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 53,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* constant parameters of sqrt(802) */
    const idx_t D   = (idx_t)802;   /* D = 802 */
    const idx_t A0  = (idx_t)28;    /* a0 = floor(sqrt(802)) = 28 */

    /* static cache of previously computed values */
    static seq_t *cache     = NULL;
    static size_t  cache_cap   = 0;      /* capacity of cache array */
    static idx_t  last_index   = (idx_t)-1;  /* highest index already stored */

    /* state for continued‑fraction calculation */
    static idx_t  m = 0;    /* numerator of fractional part */
    static idx_t  d = 1;    /* denominator of fractional part */
    static idx_t  a_val = A0;  /* current partial quotient */

    /* recurrence state for denominators */
    static seq_t q_prevprev = (seq_t)0;  /* a(-1) = 0 */
    static seq_t q_prev     = (seq_t)1;  /* a(0) = 1 */

    /* initialise cache on the first call */
    if (cache == NULL) {
        cache_cap = 1;
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* out of memory – return 0 */
        cache[0] = (seq_t)1;
        last_index = 0;
        /* m,d,a_val are already set to initial values 0,1,28 for the next step */
    }

    /* if the requested index is already cached, just return it */
    if (n <= last_index) {
        return cache[n];
    }

    /* extend the cache up to index n */
    for (idx_t i = last_index + 1; i <= n; ++i) {
        /* compute next partial quotient a_val for step i */
        m = (d * a_val) - m;
        d = (D - m * m) / d;
        a_val = (A0 + m) / d;

        /* compute next denominator using recurrence */
        seq_t next = mul_small(q_prev, (unsigned int)a_val) + q_prevprev;

        /* ensure cache has enough space */
        if ((size_t)i >= cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 4;
            while ((size_t)i >= new_cap) new_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;   /* out of memory – return 0 */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* store the new value and shift recurrence state */
        cache[i] = next;
        q_prevprev = q_prev;
        q_prev     = next;
        last_index = i;
    }

    return cache[n];
}
