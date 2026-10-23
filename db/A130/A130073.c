/*
 * A130073  Numbers k such that k divides 5^k - 3^k - 2^k = A130072(k).
 * https://oeis.org/A130073
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth positive integer k such that k divides 5^k - 3^k -
 *            2^k
 * Range:     u8 n <= 90, u16 n <= 6781, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* Static cache of found terms.  terms[0] = a(1). */
    static seq_t *terms = NULL;
    static size_t terms_len = 0;
    static size_t terms_cap  = 0;
    static idx_t  next_k = 1;  /* next integer to test */

    while (terms_len < n) {
        seq_t k = (seq_t)next_k;              /* candidate k */

        /* modular powers of 5, 3 and 2 modulo k */
        seq_t v5 = mod_pow((seq_t)5, next_k, k);
        seq_t v3 = mod_pow((seq_t)3, next_k, k);
        seq_t v2 = mod_pow((seq_t)2, next_k, k);

        /* (v5 - v3 - v2) mod k without overflow */
        seq_t diff;
        if (v5 >= v3) diff = v5 - v3;
        else          diff = v5 + k - v3;
        if (diff >= v2) diff = diff - v2;
        else            diff = diff + k - v2;

        if (diff == 0) {                      /* k satisfies the condition */
            if (terms_len == terms_cap) {      /* grow the array */
                size_t new_cap = (terms_cap == 0) ? 16 : terms_cap << 1;
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                terms = tmp;
                terms_cap = new_cap;
            }
            terms[terms_len++] = k;
        }
        ++next_k;                              /* next integer to test */
    }

    return terms[n - 1];
}
