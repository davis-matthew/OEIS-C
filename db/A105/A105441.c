/*
 * A105441  Numbers with at least two odd prime factors (not necessarily
 *          distinct).
 * https://oeis.org/A105441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth positive integer having at least two odd prime
 *            factors (counting multiplicities)
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   has_two_odd_factors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/has_two_odd_factors.h"

seq_t a(idx_t n) {
    static idx_t  *terms     = NULL;
    static idx_t   terms_cap = 0;
    static idx_t   terms_cnt = 0;
    static idx_t   next      = 2;        /* next integer to test */

    if (n == 0) return (seq_t)0;        /* not part of the sequence, safety */

    while (terms_cnt < n) {
        /* test next candidate */
        if (has_two_odd_factors(next)) {
            /* grow terms array if needed */
            if (terms_cnt >= terms_cap) {
                terms_cap = terms_cap ? terms_cap * 2 : 16;
                terms = (idx_t *)realloc(terms, terms_cap * sizeof(idx_t));
            }
            terms[terms_cnt++] = next;
        }
        ++next;
    }
    return (seq_t)terms[n - 1];
}
