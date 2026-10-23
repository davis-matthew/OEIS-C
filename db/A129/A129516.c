/*
 * A129516  Numbers k such that k divides (k-1)!! - 1.
 * https://oeis.org/A129516
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: a(n) is the n‑th integer k satisfying k | ((k‑1)!! – 1) */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    static seq_t *terms = NULL;
    static size_t terms_len = 0;
    static size_t terms_cap = 0;
    static idx_t candidate = 1;   /* next integer to test */

    while (terms_len < (size_t)n) {
        idx_t k = candidate;
        seq_t k_seq = (seq_t)k;
        seq_t prod = (seq_t)1;
        idx_t m;

        if ((k & 1) == 0) {   /* k even: product of odds 1,3,…,k-1 */
            m = k / 2;
            for (idx_t j = 0; j < m; ++j) {
                seq_t term = (seq_t)1 + ((seq_t)2 * (seq_t)j);
                prod = (prod * term) % k_seq;
            }
        } else {             /* k odd: product of evens 2,4,…,k-1 */
            m = (k - 1) / 2;
            for (idx_t j = 1; j <= m; ++j) {
                seq_t term = (seq_t)2 * (seq_t)j;
                prod = (prod * term) % k_seq;
            }
        }

        if (prod == (seq_t)1) {   /* k | ((k-1)!! – 1) */
            if (terms_len == terms_cap) {
                size_t new_cap = terms_cap ? terms_cap * 2 : 32;
                seq_t *tmp = seq_realloc(terms, terms_cap, new_cap);
                terms = tmp;
                terms_cap = new_cap;
            }
            terms[terms_len++] = (seq_t)candidate;
        }

        ++candidate;
    }

    return terms[n - 1];
}
