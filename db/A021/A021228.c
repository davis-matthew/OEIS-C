/*
 * A021228  Decimal expansion of 1/224.
 * https://oeis.org/A021228
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(10^(n+1) / 224) mod 10 which equals floor(
 *            (10^(n+1) mod 2240) / 224 )
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n-th decimal digit of 1/224 */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* 2240 = 224 * 10 */
    const uint64_t MOD = 2240ULL;
    uint64_t result = 1ULL % MOD;
    uint64_t base   = 10ULL % MOD;
    idx_t exp = n + 1;          /* exponent n+1 */

    while (exp > 0) {
        if (exp & 1) result = (result * base) % MOD;
        base = (base * base) % MOD;
        exp >>= 1;
    }

    /* result is 10^(n+1) mod 2240, always < 2240 */
    return (seq_t)(result / 224ULL);   /* gives the decimal digit (0..9) */
}
