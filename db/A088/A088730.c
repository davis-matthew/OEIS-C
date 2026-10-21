/*
 * A088730  Numbers of the form p^p - 1, where p is a prime.
 * https://oeis.org/A088730
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A051674(n) - 1 (nth prime raised to itself, minus one)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   get_prime, pow_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/pow_seq_t.h"

seq_t a(idx_t n)
{
    /* Obtain the nth prime.  n >= 1 by OEIS offset. */
    seq_t prime = (seq_t)get_prime(n);

    /* Compute prime^prime using the provided arbitrary‑precision helper. */
    seq_t p_pow = pow_seq_t(prime, (idx_t)prime);

    /* Subtract one and return. */
    return p_pow - (seq_t)1;
}
