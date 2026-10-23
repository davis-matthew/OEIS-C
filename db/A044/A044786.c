/*
 * A044786  Numbers n such that string 7,3 occurs in the base 10
 *          representation of n but not of n+1.
 * https://oeis.org/A044786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   has73
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A044786 : numbers n such that "73" appears in the decimal
 * representation of n but not in n+1.
 *
 * The function generates terms by brute force enumeration.  It
 * maintains a static cache of already-found terms so that calls with
 * arbitrary indices remain fast.
 */
#include <stdlib.h>
#include "../utils/has73.h"

seq_t a(idx_t n)
{
    /* Handle the offset of the sequence (a(1) is the first term) */
    if (n == 0)
        return (seq_t)0;   /* undefined for non-positive indices */

    /* static cache of computed terms */
    static seq_t *terms = NULL;
    static size_t terms_len = 0;
    static size_t terms_cap = 0;

    /* next integer to test */
    static seq_t cur = (seq_t)1;

    while (terms_len < (size_t)n) {
        if (has73(cur) && !has73(cur + (seq_t)1)) {
            /* store cur in the cache */
            if (terms_len == terms_cap) {
                size_t new_cap = terms_cap ? terms_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(terms,
                                            new_cap * sizeof(seq_t));
                if (!tmp) /* allocation failure */
                    return (seq_t)0;
                terms = tmp;
                terms_cap = new_cap;
            }
            terms[terms_len++] = cur;
        }
        cur++;   /* move to the next integer */
    }

    return terms[n - 1];
}
