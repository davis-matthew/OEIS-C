/*
 * A062175  a(n) = 4^(n-1) mod n.
 * https://oeis.org/A062175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4^(n-1) mod n
 * Range:     u8 n <= 264, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the offset and trivial case */
    if (n <= 1)
        return 0;

    uint64_t mod   = n;           /* modulus (n) */
    uint64_t base  = 4 % mod;     /* base modulo n */
    uint64_t exp   = n - 1;       /* exponent n-1 */
    uint64_t result = 1;          /* accumulator for modular exponentiation */

    /* binary exponentiation modulo mod */
    while (exp) {
        if (exp & 1ULL)
            result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }

    return (seq_t)result;  /* cast back to the required return type */
}
