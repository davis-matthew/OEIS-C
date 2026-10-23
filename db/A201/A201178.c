/*
 * A201178  Primes of the form 5k^3+9.
 * https://oeis.org/A201178
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*k^3 + 9 where k is the nth integer such that 5*k^3+9 is
 *            prime
 * Range:     u8 none, u16 n <= 3, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/is_prime_seq.h"

/* Find the nth prime of the form 5k^3+9. */
seq_t a(idx_t n)
{
    /* Maintain a list of found primes and the next k to test. */
    static seq_t *primes = NULL;        /* dynamic array of primes */
    static idx_t  sz      = 0;          /* number of primes stored */
    static idx_t  cap     = 0;          /* capacity of the array */
    static idx_t  next_k  = 10;         /* next k to test (start at 10) */

    /* Grow the primes array if necessary. */
    if (sz >= cap) {
        idx_t new_cap = (cap == 0) ? 4 : cap << 1;
        seq_t *new_ptr = (seq_t *)realloc(primes, new_cap * sizeof(seq_t));
        if (!new_ptr) abort();          /* allocation failed */
        primes = new_ptr;
        cap    = new_cap;
    }

    /* Generate primes until we have at least n of them. */
    while (sz < n) {
        seq_t kseq = (seq_t)next_k;
        /* Compute 5*k^3 + 9.  The operations are performed in seq_t domain. */
        const seq_t five   = 5;
        const seq_t nine   = 9;
        const seq_t kCUBE   = kseq * kseq * kseq;
        const seq_t candidate = five * kCUBE + nine;

        if (is_prime_seq(candidate)) {
            primes[sz++] = candidate;
        }
        next_k++;
    }

    /* Return the nth prime (1‑based indexing). */
    return primes[n - 1];
}
