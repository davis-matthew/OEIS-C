/*
 * A122098  Smallest number, different from 1, which when multiplied by "n"
 *          produces a number with "n" as its rightmost digits.
 * https://oeis.org/A122098
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 10^d / gcd(n, 10^d) + 1 where d = number of decimal
 *            digits of n
 * Range:     u8 n <= 99, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* compute number of decimal digits of n (n >= 1) */
    uint64_t tmp = (uint64_t)n;
    unsigned d = 0;
    while (tmp > 0) {
        ++d;
        tmp /= 10;
    }

    /* compute 10^d in seq_t */
    seq_t pow10 = (seq_t)1;
    for (unsigned i = 0; i < d; ++i)
        pow10 *= (seq_t)10;

    /* compute gcd(n, 10^d) */
    seq_t n_seq = (seq_t)n;
    seq_t g = gcd(n_seq, pow10);

    /* smallest k > 1 with n*k ≡ n (mod 10^d) */
    seq_t k = (pow10 / g) + (seq_t)1;
    return k;
}
