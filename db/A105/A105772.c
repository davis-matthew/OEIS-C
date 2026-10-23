/*
 * A105772  Numbers k such that 7*k + 2 is prime.
 * https://oeis.org/A105772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 1952, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_seq, seq_malloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sequence A105772: numbers k such that 7*k + 2 is prime.
   The sequence is generated lazily and cached, so repeated calls with
   arbitrary n are fast.  The function has external linkage as required. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_seq.h"
#include "../utils/seq_malloc.h"

seq_t a(idx_t n)
{
    if (n == 0)   /* Undefined for offset 0; return 0 for safety. */
        return 0;

    /* Static storage for computed terms */
    static seq_t *terms = NULL;        /* array of k values (seq_t)   */
    static idx_t   terms_size = 0;     /* number of terms stored      */
    static idx_t   terms_cap   = 0;     /* capacity of the array       */
    static idx_t   next_k = 0;         /* next k to test               */

    /* Ensure enough terms are stored */
    while (terms_size < n) {
        seq_t candidate = 7 * (seq_t)next_k + 2;
        if (is_prime_seq(candidate)) {
            /* Grow the terms array if necessary */
            if (terms_size + 1 > terms_cap) {
                idx_t new_cap = terms_cap ? terms_cap * 2 : 8;
                seq_t *tmp = seq_malloc(new_cap);
                if (!tmp) return 0; /* Allocation failed; return 0 as a fallback */
                for (idx_t i = 0; i < terms_size; ++i)
                    tmp[i] = terms[i];
                if (terms) free(terms);
                terms = tmp;
                terms_cap = new_cap;
            }
            terms[terms_size++] = (seq_t)next_k;
        }
        ++next_k;
    }

    return terms[n - 1];
}
