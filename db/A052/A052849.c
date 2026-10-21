/*
 * A052849  a(0) = 0; a(n) = 2*n! (n >= 1).
 * https://oeis.org/A052849
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 0 (n = 0) ; a(n) = 2 * n! for n ≥ 1
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>    /* malloc, realloc */

seq_t a(idx_t n)
{
    /* a(0) = 0 */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Cache of factorials: fact[i] = i! */
    static seq_t *fact = NULL;   /* array of factorials, fact[0] = 0! = 1 */
    static idx_t cap   = 0;      /* allocated capacity (in factorials) */
    static idx_t size  = 0;      /* number of factorials stored   (≤ cap) */

    /* --- initialise the cache on first call --- */
    if (fact == NULL) {
        size_t init = (size_t)n + 1;          /* need space for 0…n */
        fact = (seq_t *)malloc(init * sizeof(seq_t));
        if (!fact) { abort(); }              /* allocation failure */
        fact[0] = (seq_t)1;                  /* 0! = 1 */
        cap  = init;
        size = 1;
    }

    /* --- grow the cache if necessary --- */
    else if ((size_t)(n + 1) > cap) {
        size_t newcap = cap > 0 ? cap * 2 : 4;
        while (newcap < (size_t)(n + 1)) {
            newcap <<= 1;
        }
        seq_t *tmp = (seq_t *)realloc(fact, newcap * sizeof(seq_t));
        if (!tmp) { abort(); }
        fact = tmp;
        cap  = newcap;
    }

    /* --- compute missing factorials up to n --- */
    for (idx_t i = size; i <= n; ++i) {
        fact[i] = fact[i - 1] * ((seq_t)i);
    }
    size = n + 1;    /* now fact[n] is available */

    /* a(n) = 2 * n! */
    return fact[n] * ((seq_t)2);
}
