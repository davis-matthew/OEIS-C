/*
 * A106772  Primes with digit sum = 38.
 * https://oeis.org/A106772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 9, u32 n <= 4500, u64 n <= 4500,
 *            u128 n <= 4500, big any n
 * Helpers:   digit_sum_uint64, ensure_seq_array, is_prime_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of primes with digit sum = 38 */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime_uint64.h"
#include "../utils/digit_sum_uint64.h"
#include "../utils/ensure_seq_array.h"

/* a(n) returns the n‑th prime whose decimal digit sum is 38
   (OEIS A106772).  The first term is a(1)=39989.  The function
   memoises the primes found during the run so that calls may be
   made in arbitrary order. */
seq_t a(idx_t n)
{
    /* --- memoisation state ----------------------------------- */
    static seq_t *primes = NULL;   /* array of found primes          */
    static size_t cap   = 0;        /* current capacity               */
    static size_t size  = 0;        /* number of primes stored        */
    static uint64_t last_checked = 3; /* next odd candidate to test   */

    /* Expand the array enough for the needed number of primes */
    while (size < n) {
        /* Find the next odd prime */
        uint64_t p = last_checked;
        while (!is_prime_uint64(p))
            p += 2;

        /* Check the digit sum */
        if (digit_sum_uint64(p) == 38) {
            /* Ensure storage space and store */
            if (size >= cap)
                ensure_seq_array(&primes, &cap, size + 1);
            primes[size++] = (seq_t)p;
        }

        /* Move to the next odd candidate for the next iteration */
        last_checked = p + 2;
    }

    return primes[n - 1];
}
