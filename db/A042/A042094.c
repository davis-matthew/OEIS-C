/*
 * A042094  Numerators of continued fraction convergents to sqrt(571).
 * https://oeis.org/A042094
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   a(n) = numerator of the n‑th convergent of continued fraction
 *            for sqrt(571). The convergents satisfy p_{-1} = 1, p_0 = 23 p_k
 *            = a_k · p_{k-1} + p_{k-2} (k ≥ 1) where a_k are the
 *            continued‑fraction coefficients of sqrt(571)
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 38, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation keeps a static cache of computed numerators
   and generates new ones as needed, using the standard algorithm
   for continued fractions of quadratic irrationals.  It works
   for any seq_t/idx_t defined by the build system. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /*----- static state for caching and CF generation -----*/
    static seq_t *cache      = NULL;   /* array of numerators p_k   */
    static size_t cache_cap  = 0;      /* allocated size            */
    static idx_t  last_idx   = 0;      /* highest index computed    */

    /* continued‑fraction state (valid after each step) */
    static idx_t m   = 0;             /* previous m_k */
    static idx_t d   = 1;             /* previous d_k */
    static idx_t a_cf = 23;           /* a_k for current step */

    /* constants for D = 571                      */
    const idx_t D   = 571;
    const idx_t a0  = 23;   /* floor(sqrt(571)) */

    /*----- initialise cache on first call -----*/
    if (cache == NULL) {
        cache_cap = 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (cache == NULL) return (seq_t)0;  /* allocation failure */
        cache[0] = (seq_t)a0;                /* p_0 = 23              */
        last_idx = 0;
        /* m,d,a_cf already set to initial values (above) */
    }

    /*----- grow cache if required -----*/
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        while ((size_t)n >= new_cap) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (tmp == NULL) return (seq_t)0;     /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /*----- generate numerators up to index n -----*/
    while (last_idx < n) {
        /* step k = last_idx + 1 */

        /* compute next CF coefficient a_k   */
        idx_t m_next = d * a_cf - m;
        idx_t d_next = (D - m_next * m_next) / d;
        idx_t a_next = (a0 + m_next) / d_next;

        /* advance CF state */
        m   = m_next;
        d   = d_next;
        a_cf = a_next;   /* now a_cf holds a_k */

        /* compute p_k from recurrence */
        seq_t p_next;
        if (last_idx == 0) {                /* k == 1 */
            p_next = a_cf * cache[0] + (seq_t)1;   /* p_{-1}=1 */
        } else {
            p_next = a_cf * cache[last_idx] + cache[last_idx - 1];
        }

        /* store and advance indices */
        cache[++last_idx] = p_next;
    }

    return cache[n];
}
