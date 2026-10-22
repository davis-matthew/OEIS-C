/*
 * A000028  Let k = p_1^e_1 p_2^e_2 p_3^e_3 ... be the prime factorization of
 *          n. Sequence gives k such that the sum of the numbers of 1's in the
 *          binary expansions of e_1, e_2, e_3, ... is odd.
 * https://oeis.org/A000028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   search: k is a term iff sum over the prime factorization k =
 *            Product p_i^e_i of A000120(e_i) is odd; terms are enumerated by
 *            trial-division factoring of k = 2, 3, 4, ... and memoized
 * Range:     u8 n <= 125, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_term
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_term.h"

static unsigned long long *terms = NULL;
static size_t terms_cnt = 0, terms_cap = 0;
static unsigned long long next_k = 2;

seq_t a(idx_t n)
{
    while ((idx_t)terms_cnt < n) {
        if (is_term(next_k)) {
            if (terms_cnt == terms_cap) {
                size_t nc = terms_cap ? terms_cap * 2 : 1024;
                terms = (unsigned long long *)realloc(terms, nc * sizeof(unsigned long long));
                terms_cap = nc;
            }
            terms[terms_cnt++] = next_k;
        }
        next_k++;
    }
    return (seq_t)terms[n - 1];
}
