/*
 * A037417  Positive numbers having the same set of digits in base 3 and base
 *          5.
 * https://oeis.org/A037417
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 1573, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of A037417: positive integers whose
   base‑3 and base‑5 representations contain the same set of digits.
   The function searches successive integers and keeps those that
   satisfy the digit‑set condition.  A static cache stores produced
   terms, enabling arbitrary call order.  The implementation follows
   the constraints of both C11 and C++17. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Static cache of found terms and state variables. */
    static seq_t *terms = NULL;          /* dynamic array of terms   */
    static size_t terms_cap = 0;         /* capacity of the array    */
    static idx_t  terms_len = 0;         /* number of terms stored   */
    static idx_t  next_candidate = 1;    /* next integer to test     */

    if (n == 0) { /* offset is 1; guard against bad input */
        return 0;
    }

    /* Generate terms until we have at least n entries. */
    while (terms_len < n) {
        seq_t m = (seq_t)next_candidate++;
        /* compute digit set in base 3 */
        uint16_t mask3 = 0;
        seq_t t3 = m;
        while (t3 > 0) {
            int d = (int)(t3 % 3);
            mask3 |= 1 << d;
            t3 /= 3;
        }
        /* compute digit set in base 5 */
        uint8_t mask5 = 0;
        seq_t t5 = m;
        while (t5 > 0) {
            int d = (int)(t5 % 5);
            mask5 |= 1 << d;
            t5 /= 5;
        }

        /* The sets are equal only if mask5 contains only bits 0‑2
           (no digits 3 or 4) and masks match. */
        if (mask5 <= 0x7 && mask3 == mask5) {
            /* Ensure capacity of the terms array. */
            if ((size_t)terms_len >= terms_cap) {
                size_t new_cap = (terms_cap < 4) ? 4 : terms_cap * 2;
                /* Use a temporary pointer to avoid reusing the
                   keyword 'new' which is invalid in C++. */
                seq_t *new_terms = (seq_t *)realloc(terms,
                                 new_cap * sizeof(seq_t));
                if (!new_terms) {
                    /* Allocation failure; abort. */
                    abort();
                }
                terms = new_terms;
                terms_cap = new_cap;
            }
            terms[terms_len++] = m;
        }
    }

    /* Return the requested term (indexing from 1). */
    return terms[n - 1];
}
