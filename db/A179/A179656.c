/*
 * A179656  prime(n) mod (digital root(prime(n))).
 * https://oeis.org/A179656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = prime(n) mod digital_root(prime(n))
 * Range:     u8 n <= 54, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   digit_sum, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    /* Get the n-th prime (1‑based index). */
    seq_t p = (seq_t)get_prime(n);

    /* Compute the digital root of p. */
    seq_t dr = p;
    while (dr >= 10)
        dr = digit_sum(dr);

    /* Return the remainder of p divided by its digital root. */
    return p % dr;
}
