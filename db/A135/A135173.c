/*
 * A135173  a(n) = 5^p - 3^p - 2^p, where p = prime(n).
 * https://oeis.org/A135173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5^prime(n) - 3^prime(n) - 2^prime(n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 9, u128 n <= 16,
 *            big any n
 * Helpers:   big_pow, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Retrieve the nth prime (1‑indexed). */
    uint32_t p32 = get_prime(n);
    seq_t p = (seq_t)p32;

    /* Compute the three powers. */
    seq_t pow5 = big_pow((seq_t)5, p);
    seq_t pow3 = big_pow((seq_t)3, p);
    seq_t pow2 = big_pow((seq_t)2, p);

    /* Result: 5^p - 3^p - 2^p.  The subtraction is safe because 5^p > 3^p + 2^p for all primes p. */
    return pow5 - pow3 - pow2;
}
