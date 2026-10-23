/*
 * A190274  Numbers n such that n' = p^2-1, with n = semiprime = p*q, n' is
 *          the arithmetic derivative of n. Also: semiprimes of the form
 *          p*(p^2-p-1).
 * https://oeis.org/A190274
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 20, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   is_prime_big, is_prime_small, seq_alloc_array, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A190274.
   A(n) is the nth semiprime of the form p * (p² – p – 1) where
   both p and  p² – p – 1 are primes.  The function works for any
   seq_t and idx_t provided by the build system.  It uses memoisation
   so that calls may come in arbitrary order. */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/is_prime_small.h"
#include "../utils/is_prime_big.h"
#include "../utils/seq_alloc_array.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n) {
    /* static state for memoisation */
    static seq_t  *seq      = NULL;   /* array of computed terms   */
    static size_t seq_len  = 0;       /* number of terms stored    */
    static size_t seq_cap  = 0;       /* allocated capacity        */
    static uint64_t cand   = 3;       /* current integer to test   */

    while (seq_len < (size_t)n) {
        /* Find next prime p starting from cand */
        while (!is_prime_small((seq_t)cand))
            cand++;

        uint64_t p = cand;
        uint64_t q = p * p - p - 1;          /* candidate factor */

        if (q >= 2 && is_prime_big((seq_t)q)) {
            seq_t val = (seq_t)p * (seq_t)q;   /* the semiprime     */

            if (seq_len == seq_cap) {
                size_t new_cap = (seq_cap == 0) ? 16 : seq_cap * 2;
                seq_t *tmp = seq_realloc(seq, seq_cap, new_cap);
                if (!tmp) abort();            /* allocation failed */
                seq   = tmp;
                seq_cap = new_cap;
            }
            seq[seq_len++] = val;
        }
        cand++;  /* prepare for next candidate          */
    }
    return seq[n - 1];
}
