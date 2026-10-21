/*
 * A059008  Numbers n such that n^3 reversed is a prime.
 * https://oeis.org/A059008
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 1023, u32 n <= 1023, u64 n <= 1023,
 *            u128 n <= 1023, big any n
 * Helpers:   grow_seq_array, is_prime64, reverse_digits_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers n such that the reverse of n^3 is a prime. */

/* This implementation keeps a static list of found terms and expands it
   as needed.  The computation uses 64‑bit arithmetic for the cube,
   digit reversal and primality test.  Those operations are safe for the
   range of values that occur in the sequence (the n-th term grows far
   slower than the bounds of a 64‑bit unsigned integer).  The function
   works for any seq_t (8‑, 16‑, 32‑, 64‑ or 128‑bit unsigned integer
   or arbitrary‑precision integer type) because the intermediate
   calculations are performed in a fixed 64‑bit type and the final term
   is cast into seq_t before being stored. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime64.h"
#include "../utils/grow_seq_array.h"
#include "../utils/reverse_digits_uint64.h"

/* Main sequence function.  The first term (n==1) is 5. */
seq_t a(idx_t n)
{
    static seq_t *terms = NULL;      /* stored sequence values      */
    static size_t terms_cap = 0;     /* current capacity of array   */
    static size_t terms_len = 0;     /* number of stored terms      */
    static uint64_t next_candidate = 1; /* next n to test              */

    if (n == 0) {                    /* undefined, but avoid UB     */
        return (seq_t)0;
    }

    /* Expand the term list until we have at least n terms. */
    while (terms_len < (size_t)n) {
        uint64_t cand = next_candidate++;
        uint64_t cube = cand * cand;
        cube = cube * cand;          /* compute n^3 as 64‑bit integer */

        uint64_t rev = reverse_digits_uint64(cube);
        if (is_prime64(rev)) {
            /* Grow the array if necessary. */
            if (terms_len == terms_cap) {
                size_t new_cap = (terms_cap == 0) ? 16 : terms_cap * 2;
                if (new_cap < terms_len + 1) new_cap = terms_len + 1;
                grow_seq_array(&terms, &terms_cap, new_cap);
            }
            terms[terms_len++] = (seq_t)cand;
        }
    }

    return terms[n - 1];
}
