/*
 * A218075  a(n) = 2^(prime(n+1) - prime(n)).
 * https://oeis.org/A218075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(prime(n+1)-prime(n))
 * Range:     u8 n <= 23, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   get_prime, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n) {
    /* a(1) is defined; for n < 1 we return 0 (undefined behaviour
       for the sequence is irrelevant). */
    if (n < 1) return (seq_t)0;

    /* For 1‑indexed primes: prime(1)=2, prime(2)=3, ...  */
    idx_t diff = (idx_t)(get_prime(n + 1u) - get_prime(n));

    return pow_uint(2u, diff);
}
