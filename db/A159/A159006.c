/*
 * A159006  Transformation of prime(n): flip digits in the binary
 *          representation, revert the sequence of digits, and convert back to
 *          decimal.
 * https://oeis.org/A159006
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = A036044(prime(n))
 * Range:     u8 n <= 54, u16 n <= 7527, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bitlen_seq_t, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   a(n) is obtained by taking the n-th prime, flipping each binary
 *   digit (0→1, 1→0), then reversing the bit string and interpreting
 *   the result as a decimal integer. */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/bitlen_seq_t.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* defined only for n >= 1 */
    seq_t p = (seq_t)get_prime(n);    /* n-th prime, 1‑indexed */

    unsigned len = bitlen_seq_t(p);   /* number of bits in prime */
    seq_t result = 0;

    for (unsigned i = 0; i < len; ++i) {
        seq_t bit = (p >> i) & 1;      /* original bit (LSB first) */
        result = (result << 1) | (bit ^ 1);  /* flipped + reversed */
    }
    return result;
}
