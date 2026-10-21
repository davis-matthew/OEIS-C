/*
 * A212158  a(n) = ((prime(n) - 1)/2)!, n >= 2.
 * https://oeis.org/A212158
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((prime(n) - 1)/2)! (n >= 2)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 9, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   factorial, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"   /* provides uint32_t get_prime(idx_t k) */
#include "../utils/factorial.h"  /* provides seq_t factorial(idx_t n) */

/* Return the n-th term of the sequence A212158.
   For n < 2 the function returns 0 (offset behaviour is irrelevant). */
seq_t a(idx_t n)
{
    if (n < 2) {
        return 0;   /* undefined for n < 2; behaviour does not matter */
    }

    /* Obtain the n-th prime.  get_prime works for n up to the required limits. */
    uint32_t p = get_prime(n);

    /* Compute (p - 1)/2 as an idx_t.  p is guaranteed even after subtracting 1. */
    idx_t k = (idx_t)(p - 1) / 2;

    /* Return factorial(k) as seq_t.  factorial handles arbitrary size. */
    return factorial(k);
}
