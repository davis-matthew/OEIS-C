/*
 * A103998  Number of dimer tilings of a 4 x 2n Moebius strip.
 * https://oeis.org/A103998
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=11, a(2)=71, a(3)=539, a(4)=4271,
 *            a(n)=11*a(n-1)-25*a(n-2)+11*a(n-3)-a(n-4) for n>=5 (the term for
 *            n=4 includes an additional -3 from the generating function)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static table for memoisation */
    static seq_t *tbl = NULL;
    static idx_t tbl_size = 0;    /* allocated size */
    static idx_t max_computed = 0;/* highest index computed */

    /* initialise base values */
    if (tbl == NULL) {
        /* allocate space for indices 0..3 (base) */
        tbl = (seq_t *)malloc(4 * sizeof(seq_t));
        if (tbl == NULL) abort();
        tbl[0] = (seq_t)1;
        tbl[1] = (seq_t)11;
        tbl[2] = (seq_t)71;
        tbl[3] = (seq_t)539;
        tbl_size    = 4;
        max_computed = 3;
    }

    /* if requested index already known, return it */
    if (n <= max_computed) {
        return tbl[n];
    }

    /* expand table if needed */
    if (tbl_size <= n) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(tbl, new_size * sizeof(seq_t));
        if (tmp == NULL) abort();
        tbl = tmp;
        tbl_size = new_size;
    }

    /* compute missing terms up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t value;
        if (i == 4) {
            /* special case: a(4) = 11*a3 - 25*a2 + 11*a1 - a0 - 3 */
            value = 11 * tbl[i - 1];
            value -= 25 * tbl[i - 2];
            value += 11 * tbl[i - 3];
            value -= tbl[i - 4];
            value -= (seq_t)3;
        } else {
            value = 11 * tbl[i - 1];
            value -= 25 * tbl[i - 2];
            value += 11 * tbl[i - 3];
            value -= tbl[i - 4];
        }
        tbl[i] = value;
    }

    max_computed = n;
    return tbl[n];
}
