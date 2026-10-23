/*
 * A044326  Numbers n such that string 8,3 occurs in the base 9 representation
 *          of n but not of n-1.
 * https://oeis.org/A044326
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   has83
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search-based implementation for OEIS A044326:
   the n‑th integer whose base‑9 representation contains the
   substring 8,3 but whose predecessor does not. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has83.h"

/* Return the n-th term of A044326 (offset 1). */
seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* undefined for n < 1 */

    /* Static cache for already computed terms. */
    static seq_t *terms = NULL;
    static size_t terms_len = 0;   /* number of terms stored */
    static size_t terms_cap = 0;   /* allocated capacity       */
    static seq_t next_num = 1;     /* next candidate number    */

    if (n > terms_len) {           /* need more terms           */
        if (terms == NULL) {       /* first allocation          */
            terms_cap = 128;
            terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
        }
        while (terms_len < n) {    /* compute until we have n   */
            seq_t num = next_num;
            if (has83(num) && !has83(num - 1)) {
                if (terms_len == terms_cap) { /* grow buffer    */
                    terms_cap <<= 1;
                    terms = (seq_t *)realloc(terms,
                                              terms_cap * sizeof(seq_t));
                }
                terms[terms_len++] = num;
            }
            next_num++;
        }
    }
    return terms[n - 1];
}
