/*
 * A225063  Smallest k such that (k-1)*prime(n) +/- k are both prime.
 * https://oeis.org/A225063
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   get_prime, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS sequence A225063:
   a(n) = smallest k such that both (k-1)·prime(n) – k  and
   (k-1)·prime(n) + k are prime. */

/* Include helpers that provide the nth prime and a generic
   primality test. */
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    /* nth prime (p_1 = 2, p_2 = 3, ...) */
    seq_t p = (seq_t)get_prime(n);

    /* Search for the smallest k >= 2 satisfying the condition. */
    for (idx_t k = 2; ; ++k) {
        seq_t kk  = (seq_t)k;        /* k */
        seq_t km1 = kk - 1;          /* k-1 */

        /* (k-1)*p - k  and  (k-1)*p + k */
        seq_t v1 = km1 * p - kk;
        seq_t v2 = km1 * p + kk;

        /* Both must be prime.  The function works for any seq_t
           implementation (uint64_t, __int128, or arbitrary precision). */
        if (isPrimeGeneric(v1) && isPrimeGeneric(v2))
            return kk;
    }
}
