/*
 * A072341  a(n) = the least natural number k such that k*sigma(n) + 1 is
 *          prime.
 * https://oeis.org/A072341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the least k such that k*sigma(n)+1 is prime
 * Range:     u8 n <= 15, u16 n <= 6737, u32 n <= 9481, u64 n <= 9481,
 *            u128 n <= 9481, big any n
 * Helpers:   isPrimeGeneric, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/sigma_of.h"
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    /* Ensure we only compute for valid indices; offset is 1. */
    if (n == 0)
        return 0;               /* undefined for n=0, return 0 */

    /* Compute σ(n).  The helper returns a seq_t value. */
    seq_t s = sigma_of(n);

    /* Find the smallest k such that k*s + 1 is prime. */
    seq_t k = 1;
    for (;; ++k) {
        seq_t v = s * k + 1;    /* v = σ(n)*k + 1 */
        if (isPrimeGeneric(v))
            return k;
    }
}
