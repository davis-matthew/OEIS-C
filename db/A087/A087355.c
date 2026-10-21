/*
 * A087355  n^10 mod 10^n.
 * https://oeis.org/A087355
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^10 mod 10^n (OEIS A087355)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   pow10, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* For n < 10 the modulus 10^n is larger than n^10,
       so we can compute the remainder directly. */
    if (n < 10) {
        seq_t val = pow_uint((uint64_t)n, 10);
        seq_t mod = pow10(n);
        return val % mod;
    }

    /* For n == 10, n^10 == 10^10, so the result is 0. */
    if (n == 10) {
        return (seq_t)0;
    }

    /* For n > 10, 10^n grows much faster than n^10,
       thus n^10 mod 10^n == n^10. */
    return pow_uint((uint64_t)n, 10);
}
