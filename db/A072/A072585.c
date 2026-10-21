/*
 * A072585  Numbers k such that k and phi(k) end with the same digit in base
 *          10.
 * https://oeis.org/A072585
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   last_digit_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation generates the numbers k such that k and φ(k)
   end with the same last decimal digit.  It keeps a static cache of
   found terms and expands it as needed.  For each candidate k the
   function computes φ(k) modulo 10 only, using integer factorisation
   with 64‑bit arithmetic.  The algorithm runs in O(n·√k) time, which
   is well below the 2‑second limit for n ≈ 10 000. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/last_digit_phi.h"

/* Returns the n‑th term of sequence A072585 (1‑based). */
seq_t a(idx_t n)
{
    /* static cache of already found terms */
    static seq_t *terms   = NULL;
    static size_t cap     = 0;
    static size_t count   = 0;

    /* next candidate integer to test */
    static idx_t next_k   = 1;

    /* generate terms until we have at least n of them */
    while (count < n) {
        unsigned char last_k   = (unsigned char)(next_k % 10ULL);
        unsigned char last_phi = last_digit_phi(next_k);

        if (last_k == last_phi) {
            /* ensure enough capacity */
            if (count >= cap) {
                size_t new_cap = cap ? cap * 2 : 16;
                seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
                if (!new_arr) return (seq_t)0;           /* allocation failure */
                for (size_t i = 0; i < count; ++i) new_arr[i] = terms[i];
                free(terms);
                terms = new_arr;
                cap   = new_cap;
            }
            terms[count++] = (seq_t)next_k;
        }
        ++next_k;
    }
    return terms[n - 1];
}
