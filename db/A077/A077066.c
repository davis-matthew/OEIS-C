/*
 * A077066  Squarefree kernel of prime(n) + 1.
 * https://oeis.org/A077066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = rad(prime(n)+1)
 * Range:     u8 n <= 54, u16 n <= 6545, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, rad_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the helper functions get_prime and rad_of from the utilities library. */

#include "../utils/get_prime.h"
#include "../utils/rad_of.h"

seq_t a(idx_t n)
{
    /* a(1) = rad(prime(1)+1) = rad(2+1) = 3, as required. */
    if (n < 1)
        return 0;          /* undefined for n < 1 */

    /* Obtain the n-th prime (1‑based). */
    seq_t p = get_prime(n);

    /* Compute the radical of p + 1. */
    idx_t v = (idx_t)(p + 1);   /* cast to idx_t (uint64_t) before passing to rad_of */
    return rad_of(v);
}
