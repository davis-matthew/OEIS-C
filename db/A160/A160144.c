/*
 * A160144  Numerator of (2*n+1)/(2^(2*n+1)-1).
 * https://oeis.org/A160144
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, frac, nonn
 * Formula:   a(n) = numerator of (2*n+1)/(2^(2*n+1)-1)
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, mul_mod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/mul_mod.h"

/* Helper: multiplication modulo mod. Works for all types when the
   built-in multiplication operand size is sufficient; for arbitrary
   precision seq_t the overloaded operators handle it correctly. */

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n * 2u + 1;          /* 2*n + 1 */

    /* Compute 2^(2*n+1) modulo m using binary exponentiation. */
    seq_t result = 1 % m;
    seq_t base   = 2 % m;
    unsigned __int128 exp = (unsigned __int128)n * 2u + 1u;  /* 2*n+1 */
    while (exp)
    {
        if (exp & 1u)
            result = mul_mod(result, base, m);
        exp >>= 1;
        if (exp)
            base = mul_mod(base, base, m);
    }

    /* t = (2^(2*n+1) - 1) mod m */
    seq_t t = (result + m - 1) % m;

    /* g = gcd(m, t) */
    seq_t g = gcd(m, t);

    /* Numerator = (2*n+1) / g */
    return m / g;
}
