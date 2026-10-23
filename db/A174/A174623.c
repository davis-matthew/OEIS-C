/*
 * A174623  (2^p+1)^2 where p is prime.
 * https://oeis.org/A174623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2^p + 1)^2 where p is the nth prime (p_1 = 2)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 18,
 *            big any n
 * Helpers:   get_prime, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    /* Get the nth prime (1‑based). */
    uint32_t pr = get_prime(n);

    /* Compute 2^p. */
    seq_t two_to_p = pow2(pr);

    /* Compute (2^p + 1)^2. */
    seq_t tmp = two_to_p + (seq_t)1;
    return tmp * tmp;
}
