/*
 * A110606  Numbers n whose base 8 representations, interpreted as base 10
 *          integers, are semiprimes.
 * https://oeis.org/A110606
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   alloc_seq_array, base8_to_decimal, is_semiprime_uint64,
 *            resize_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers n whose base‑8 representation, interpreted as a 
   decimal integer, is a semiprime.  */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_semiprime_uint64.h"
#include "../utils/alloc_seq_array.h"
#include "../utils/resize_array.h"
#include "../utils/base8_to_decimal.h"

/* Return the n‑th term of the sequence.  */
seq_t a(idx_t n)
{
    if (n == 0) /* undefined for index 0; placeholder */
        return (seq_t)0;

    /* memoisation storage */
    static seq_t *terms      = NULL;
    static size_t terms_cap  = 0;
    static idx_t terms_size  = 0;
    static idx_t last_candidate = 1;   /* next integer to test */

    /* grow the storage if necessary */
    if (terms_size < n) {
        if (terms_cap < (size_t)n) {
            size_t new_cap = terms_cap ? terms_cap * 2 : 16;
            while (new_cap < (size_t)n)
                new_cap <<= 1;
            if (terms == NULL)
                terms = alloc_seq_array(new_cap);
            else
                terms = resize_array(terms, terms_cap, new_cap);
            terms_cap = new_cap;
        }

        /* generate terms until we have n of them */
        while (terms_size < n) {
            for (;;) {
                seq_t f = base8_to_decimal(last_candidate);
                if (is_semiprime_uint64((uint64_t)f)) {
                    terms[terms_size++] = last_candidate;
                    break;
                }
                ++last_candidate;   /* next integer to test */
            }
            ++last_candidate;       /* start next search at next integer */
        }
    }
    return terms[n - 1];
}
