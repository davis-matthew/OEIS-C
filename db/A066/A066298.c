/*
 * A066298  a(n) = googol (mod n).
 * https://oeis.org/A066298
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10^100 mod n (n ≥ 1)
 * Range:     u8 n <= 264, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t */

seq_t a(idx_t n)
{
    /* a(1) = 0, a(n) = 10^100 mod n for n ≥ 2 */
    if (n <= 1)
        return (seq_t)0;

    seq_t mod   = (seq_t)n;   /* n is the modulus */
    seq_t base  = (seq_t)10;  /* base 10 */
    seq_t result = (seq_t)1 % mod;  /* 1 mod n  */
    /* binary exponentiation – exponent is 100 (uint64_t) */
    uint64_t exp = 100;
    seq_t cur = base % mod;

    while (exp != 0) {
        if (exp & 1ULL)
            result = (result * cur) % mod;
        cur = (cur * cur) % mod;
        exp >>= 1ULL;
    }

    return result;
}
