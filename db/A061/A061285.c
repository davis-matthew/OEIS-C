/*
 * A061285  a(n) = 2^((prime(n) - 1)/2).
 * https://oeis.org/A061285
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^((prime(n) - 1)/2)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 18, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   _pow_seq_t, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/_pow_seq_t.h"

seq_t a(idx_t n) {
    /* The sequence is defined for n ≥ 2.  For n < 2 return 0. */
    if (n < 2)
        return (seq_t)0;

    /* Get the n‑th prime (1‑based). */
    uint32_t p = get_prime(n);

    /* exponent = (p - 1) / 2 */
    idx_t exp = ((idx_t)p - 1) >> 1;

    /* Compute 2^exp using binary exponentiation provided by the utilities. */
    return _pow_seq_t((seq_t)2, exp);
}
