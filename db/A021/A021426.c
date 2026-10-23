/*
 * A021426  Decimal expansion of 1/422.
 * https://oeis.org/A021426
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor( 10^(n+1) / 422 ) mod 10
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* That is equivalent to a(n) = (10^(n+1) mod 4220) / 422 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* 4220 is the modulus 422 * 10; it fits in 16 bits. */
    const uint64_t MOD   = 4220ULL;
    const uint64_t DIV   = 422ULL;
    uint64_t exp   = (uint64_t)n + 1;          /* exponent n+1 */
    uint64_t base  = 10ULL % MOD;              /* base 10 modulo MOD */
    uint64_t result = 1ULL;                    /* current power modulo MOD */

    /* fast exponentiation: compute 10^(n+1) mod MOD */
    while (exp != 0) {
        if (exp & 1ULL)
            result = (result * base) % MOD;
        base = (base * base) % MOD;
        exp >>= 1ULL;
    }

    /* result is 10^(n+1) mod 4220; divide by 422 to get the digit. */
    seq_t digit = (seq_t)(result / DIV);
    return digit;
}
