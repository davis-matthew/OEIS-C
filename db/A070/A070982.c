/*
 * A070982  Smallest integer k such that n divides sigma(k).
 * https://oeis.org/A070982
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 1666, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A070982:  a(n) = smallest integer k such that n divides sigma(k)
 *  This implementation simply tests successive k starting from 1
 *  until sigma(k) mod n == 0, which works efficiently for all n
 *  up to about 10 000.  The function sigma_of is provided by the
 *  utils library. */
#include <stdint.h>
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;          /* sigma(1) == 1, so a(1) = 1 */

    seq_t N = (seq_t)n;           /* cast once to avoid repeated casts */

    /* Search k = 1, 2, 3, ... until sigma(k) is divisible by n. */
    for (idx_t k = 1; ; ++k) {
        seq_t s = sigma_of(k);   /* sigma(k) as seq_t */
        if ((s % N) == 0)
            return (seq_t)k;     /* found the minimal k */
    }
}
