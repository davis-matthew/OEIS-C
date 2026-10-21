/*
 * A070344  a(n) = 3^n mod 29.
 * https://oeis.org/A070344
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n mod 29
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes 3^n modulo 29.  The argument n is an idx_t (unsigned 64‑bit integer);
   seq_t may be any unsigned integer type, including arbitrary‑precision. */
seq_t a(idx_t n)
{
    /* Reduce exponent using Fermat's little theorem: 3^28 ≡ 1 (mod 29) */
    idx_t e = n % 28;

    seq_t result = 1;
    seq_t base   = 3;
    seq_t mod    = 29;

    while (e != 0) {
        if (e & 1)
            result = (result * base) % mod;
        base = (base * base) % mod;
        e >>= 1;
    }
    return result;
}
