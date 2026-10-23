/*
 * A191561  a(n) = 2^n mod 3*n.
 * https://oeis.org/A191561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n mod (3*n)
 * Range:     u8 n <= 86, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1, but we handle n==0
       harmlessly by returning 0. */
    if (n == 0)
        return 0;

    /* Compute modulus (3*n) in a wider integer type to avoid overflow
       when n is large, then convert to seq_t. */
    idx_t mod64 = n * 3;
    seq_t mod = (seq_t)mod64;

    /* Modular exponentiation: compute 2^n mod mod. */
    seq_t result = 1;
    seq_t base = (seq_t)2;

    while (n > 0) {
        if (n & 1)
            result = (result * base) % mod;
        base = (base * base) % mod;
        n >>= 1;
    }

    return result;
}
