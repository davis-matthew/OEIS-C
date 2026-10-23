/*
 * A042801  Denominators of continued fraction convergents to sqrt(931).
 * https://oeis.org/A042801
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 21, u64 n <= 43, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Denominators of continued‑fraction convergents to sqrt(931) */
#include <stdlib.h>

/*
 * seq_t a(idx_t n)
 *
 * Computes the nth denominator of the convergents of √931.
 * The convergents are generated from the continued fraction
 * expansion of √931; the recurrence for the denominators q_k
 * is
 *
 *   q_(-1) = 0,  q_0 = 1,
 *   q_k   = a_k * q_(k-1) + q_(k-2),
 *
 * where a_k are the partial quotients of √931.
 * The partial quotients are produced by the standard
 * recurrence
 *
 *   m_(k+1) = a_k*d_k - m_k
 *   d_(k+1) = (931 - m_(k+1)^2) / d_k
 *   a_(k+1) = floor((30 + m_(k+1)) / d_(k+1))
 *
 * with m_0 = 0, d_0 = 1, a_0 = floor(√931) = 30.
 */

seq_t a(idx_t n)
{
    /* static state */

    static seq_t *cache = NULL;   /* array of computed denominators */
    static size_t cap   = 0;      /* capacity of cache array          */
    static size_t len   = 0;      /* number of elements currently
                                   * stored in cache (len > 0 after
                                   * first initialization)      */

    /* partial‑quotient generators for √931 */
    static idx_t m      = 0;            /* m_k            */
    static idx_t d      = 1;            /* d_k            */
    static idx_t a_next = 30;           /* a_k, with a_0 = 30 */

    /* recurrence for denominators */
    static seq_t prev1 = 0;            /* q_(k-2)   (q_-1 at start)   */
    static seq_t prev2 = 1;            /* q_(k-1)   (q_0  at start)   */

    /* constants */
    const idx_t D  = 931;              /* discriminant    */
    const idx_t A0 = 30;               /* floor(√931)     */

    /* initialize cache on first use */
    if (len == 0) {
        /* allocate space for the first term */
        cache = (seq_t*)malloc(sizeof(seq_t));
        if (!cache)  return 0; /* failure: caller may check for 0 */

        cache[0] = prev2;   /* a(0) = 1 */
        len   = 1;
        cap   = 1;
    }

    /* generate terms until the requested index is reached */
    while (len <= n) {
        /* produce next partial quotient */
        m      = d * a_next - m;
        d      = (D - m * m) / d;
        a_next = (A0 + m) / d;

        /* compute next denominator with recurrence */
        seq_t q_new = a_next * prev2 + prev1;

        /* resize cache if necessary */
        if (len >= cap) {
            size_t newcap = cap ? (cap << 1) : 4;
            seq_t *tmp    = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
            if (!tmp)  return 0; /* failure */
            cache = tmp;
            cap   = newcap;
        }

        /* store new term */
        cache[len] = q_new;
        len++;

        /* advance recurrence */
        prev1 = prev2;
        prev2 = q_new;
    }

    return cache[n];
}
