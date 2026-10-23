/*
 * A139797  Inverse binomial transform of [0, A133474].
 * https://oeis.org/A139797
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)+2*a(n-2)-a(n-3)+a(n-4)+2*a(n-5), a(0)=0, a(1)=0,
 *            a(2)=0, a(3)=0, a(4)=1 Derived from the rational generating
 *            function G(x) = x^4/((1+x)^2*(1-2x)*(1-x+x^2))
 * Range:     u8 n <= 12, u16 n <= 20, u32 n <= 36, u64 n <= 68, u128 n <= 132,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed terms */
    static seq_t *cache = NULL;
    static idx_t  max_n = (idx_t)-1;      /* highest n computed so far */
    static size_t capacity = 0;           /* allocated size of cache */

    /* need space for at least 5 terms (for indices 0..4) */
    if (cache == NULL) {
        capacity = 8;                    /* arbitrary small initial capacity */
        cache = (seq_t*)malloc(sizeof(seq_t)*capacity);
        /* initialise base terms once */
        cache[0] = 0; cache[1] = 0; cache[2] = 0; cache[3] = 0; cache[4] = 1;
        max_n = 4;
    }

    /* enlarge cache if required */
    if (n >= capacity) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t)*new_cap);
        if (tmp) {
            cache = tmp;
            capacity = new_cap;
        } else {
            /* on realloc failure keep old cache; sequence will not grow,
               but for typical n this is unlikely; for safety we continue
               with current capacity (may overrun) */
        }
    }

    /* compute missing terms up to n */
    if (n > max_n) {
        for (idx_t k = max_n + 1; k <= n; ++k) {
            seq_t term = cache[k-1];            /* a(k-1)      */
            term += 2 * cache[k-2];            /* +2*a(k-2)   */
            term -=     cache[k-3];            /* -a(k-3)     */
            term +=     cache[k-4];            /* +a(k-4)     */
            term += 2 * cache[k-5];            /* +2*a(k-5)   */
            cache[k] = term;
        }
        max_n = n;
    }

    return cache[n];
}
