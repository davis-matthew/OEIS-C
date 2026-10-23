/*
 * A155939  Numbers k such that 13*k + 8 is not a prime.
 * https://oeis.org/A155939
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th integer k (k >= 0) for which 13*k + 8 is not prime
   by iterating k and testing primality of the linear value.  */
#include <stdint.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;    /* undefined offset; return 0 */
    }

    idx_t count = 0;
    idx_t k = 0;

    /* Linear search */
    while (count < n) {
        /* compute 13*k + 8 in a wider type to avoid overflow during
           the multiplication, then cast to seq_t for the primality test */
        uint64_t val64 = (uint64_t)13 * (uint64_t)k + 8ULL;
        seq_t  val = (seq_t)val64;

        if (!is_prime_big(val)) {    /* composite (or 1) */
            ++count;
            if (count == n) {
                return (seq_t)k;
            }
        }
        ++k;
    }

    /* unreachable, but return something to avoid a warning */
    return (seq_t)0;
}
