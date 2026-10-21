/*
 * A054056  Numbers of form 29+n^2+n or 29+2*n^2.
 * https://oeis.org/A054056
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n‑th smallest integer of the form 29 + n^2 + n or 29
 *            + 2*n^2. The sequence is the sorted union of the two quadratic
 *            sequences, with duplicates removed
 * Range:     u8 n <= 23, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* static cache: increasing array of all computed terms */
    static seq_t *cache = NULL;
    static size_t cap = 0;    /* allocated capacity */
    static size_t sz  = 0;    /* number of stored terms */

    /* current indices of the two quadratic sequences */
    static idx_t i = 0;   /* index for 29 + i*(i+1)   */
    static idx_t j = 0;   /* index for 29 + 2*j*j     */

    /* ensure cache has some space */
    if (cache == NULL) {
        cap = 64;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
    }

    /* generate terms until we have at least n of them */
    while (sz < n) {
        /* next candidates from the two formulas */
        unsigned long long v1 = 29ULL + (unsigned long long)i * ((unsigned long long)i + 1ULL);
        unsigned long long v2 = 29ULL + 2ULL * (unsigned long long)j * (unsigned long long)j;

        unsigned long long next_val;
        if (v1 < v2) {
            next_val = v1;
            i++;
        } else if (v2 < v1) {
            next_val = v2;
            j++;
        } else {                /* v1 == v2 : duplicate */
            next_val = v1;
            i++; j++;
        }

        /* store the value if it is not a duplicate of the last one */
        if (sz == 0 || (seq_t)next_val != cache[sz-1]) {
            if (sz == cap) {                            /* grow cache */
                size_t newcap = cap << 1;
                seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE);
                cache = tmp;
                cap = newcap;
            }
            cache[sz++] = (seq_t)next_val;
        }
    }

    return cache[n-1];
}
