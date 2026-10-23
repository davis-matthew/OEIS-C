/*
 * A140496  a(n) = p - number of digits of p^4, where p = n-th prime.
 * https://oeis.org/A140496
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = p - number of digits of p^4, where p = n-th prime
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 54, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   digit_len, get_prime, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/pow_seq.h"
#include "../utils/digit_len.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for 0, safe fallback */

    /* Obtain the n-th prime.  get_prime returns a uint32_t. */
    uint32_t p32 = get_prime(n);

    /* Convert to seq_t for arbitrary-precision arithmetic. */
    seq_t p = (seq_t)p32;

    /* Compute p^4.  pow_seq takes an exponent as an int. */
    seq_t p4 = pow_seq(p, 4);

    /* Count decimal digits of p^4.  digit_len works for arbitrary
       precision seq_t values. */
    int d = digit_len(p4);

    /* Result: p - number_of_digits(p^4).  Cast d to seq_t before subtract. */
    return p - (seq_t)d;
}
