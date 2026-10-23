/*
 * A139738  a(n) = 10^n mod 8^n.
 * https://oeis.org/A139738
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10^n mod 8^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

static seq_t mod_pow_two(seq_t base, idx_t exp, idx_t bits)
{
    /* Compute base^exp mod 2^bits using binary exponentiation.
       mask = 2^bits - 1  (only the lower 'bits' bits matter).   */
    seq_t mask = pow_uint(2, bits);
    mask = mask - 1;
    seq_t result = 1;
    seq_t b = base & mask;
    idx_t e = exp;
    while (e > 0) {
        if (e & 1)
            result = (result * b) & mask;
        b = (b * b) & mask;
        e >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* 10^0 mod 8^0 is defined as 0 here */
    seq_t r = mod_pow_two((seq_t)5, n, 2 * n);  /* 5^n mod 2^(2n)  */
    seq_t two_n = pow_uint(2, n);               /* 2^n              */
    return r * two_n;                           /* 2^n * r mod 2^(3n) == 10^n mod 8^n */
}
