/*
 * A021942  Decimal expansion of 1/938.
 * https://oeis.org/A021942
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(10^(n+1) / 938) % 10
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is the nth digit after the decimal in the expansion of 1/938. */
#include <stdint.h>

seq_t a(idx_t n)
{
    const uint64_t MOD = 9380ULL;   /* 938 * 10, so that (10^(n+1) mod 9380) / 938
                                      gives the required digit */
    uint64_t result = 1ULL;
    uint64_t base   = 10ULL % MOD;
    idx_t    exp    = n + 1ULL;     /* exponent may overflow for very large n;
                                      but that case is outside the expected range */

    while (exp > 0) {
        if (exp & 1ULL)
            result = (result * base) % MOD;
        base = (base * base) % MOD;
        exp >>= 1;
    }
    return (seq_t)(result / 938ULL);   /* digit is 0..9 */
}
