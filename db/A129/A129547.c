/*
 * A129547  a(n)= n! - (n-1)!! - a(n-1), with a(1)=1!-0!!=0.
 * https://oeis.org/A129547
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)! - n!! - a(n-1), a(0) = 0
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static caches */
    static idx_t  cached_n = 0;      /* highest n computed for a[] */
    static seq_t *fact  = NULL;      /* factorial[i] = i!        (size: cached_n+2) */
    static seq_t *dfact = NULL;      /* double factorial[i] = i!! (size: cached_n+1) */
    static seq_t *val   = NULL;      /* a[i] values               (size: cached_n+1) */

    /* trivial case */
    if (n == 0)
        return (seq_t)0;

    /* expand caches if needed */
    if (n > cached_n) {
        idx_t new_n = n;

        /* sizes needed after expansion */
        size_t fact_sz   = new_n + 2;   /* indices 0 .. new_n+1          */
        size_t dfact_sz  = new_n + 1;   /* indices 0 .. new_n            */
        size_t val_sz    = new_n + 1;   /* indices 0 .. new_n            */

        /* realloc or allocate */
        seq_t *f = (seq_t*)realloc(fact,   fact_sz   * sizeof(seq_t));
        seq_t *d = (seq_t*)realloc(dfact,  dfact_sz  * sizeof(seq_t));
        seq_t *v = (seq_t*)realloc(val,    val_sz    * sizeof(seq_t));
        if (!f || !d || !v)
            return (seq_t)0;   /* allocation failure */

        fact  = f;
        dfact = d;
        val   = v;

        /* initialise factorial[0] if not set */
        if (fact[0] == (seq_t)0)
            fact[0] = (seq_t)1;

        /* compute factorials up to new_n+1 */
        for (idx_t i = 1; i <= new_n + 1; ++i)
            fact[i] = fact[i - 1] * (seq_t)i;

        /* initialise double factorials */
        dfact[0] = (seq_t)1;
        if (new_n >= 1)
            dfact[1] = (seq_t)1;
        for (idx_t i = 2; i <= new_n; ++i)
            dfact[i] = (seq_t)i * dfact[i - 2];

        /* initialise a[0] */
        val[0] = (seq_t)0;

        /* compute a[i] for i = 1 .. new_n */
        for (idx_t i = 1; i <= new_n; ++i)
            val[i] = fact[i + 1] - dfact[i] - val[i - 1];

        cached_n = new_n;
    }

    return val[n];
}
