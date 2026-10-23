/*
 * A109845  a(1) = 2; a(2n) = least common multiple of all previous terms + 1.
 *          a(2n+1) = least common multiple of all previous terms - 1.
 * https://oeis.org/A109845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1) = 2; a(n) = a(n-1)^2 + (-1)^n * a(n-1) + (-1)^n (n ≥ 2)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms; cache[0] holds a(1) */
    static seq_t *cache = NULL;
    static idx_t cap = 0;   /* allocated size */
    static idx_t len = 0;   /* number of terms currently stored */

    /* initialise cache on first call */
    if (cache == NULL) {
        cap = 8;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) abort();          /* allocation failure */
        cache[0] = (seq_t)2;                 /* a(1) */
        cache[1] = (seq_t)3;                 /* a(2) */
        len = 2;
    }

    /* already computed? */
    if (n <= len)
        return cache[n - 1];

    /* grow cache to accommodate up to n */
    while (len < n) {
        if (len >= cap) {
            /* double capacity */
            cap = (cap == 0) ? 8 : cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            if (tmp == NULL) abort();
            cache = tmp;
        }

        /* compute next term: a(len+1)  */
        const seq_t prev = cache[len - 1];
        seq_t term = prev * prev;

        /* n = len + 1 */
        if (((len + 1) & 1) == 0) {          /* even n: add */
            term += prev + 1;
        } else {                              /* odd n: subtract */
            term -= prev + 1;
        }

        cache[len++] = term;                 /* store newly computed term */
    }

    return cache[n - 1];
}
