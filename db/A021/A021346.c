/*
 * A021346  Decimal expansion of 1/342.
 * https://oeis.org/A021346
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor((10^(n+1))/342) mod 10
 * Range:     u8 n <= 1, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses an equivalent expression a(n) = (10^(n+1) mod 3420) / 342 */

#include <stdint.h>   /* for uint64_t */

static seq_t pow10_mod(idx_t exp)
{
    const uint64_t MOD = 3420ULL;   /* 342 * 10 */
    uint64_t result = 1ULL;
    uint64_t base   = 10ULL % MOD;

    while (exp)
    {
        if (exp & 1ULL)
            result = (result * base) % MOD;
        base = (base * base) % MOD;
        exp >>= 1ULL;
    }
    return (seq_t)result;   /* result < 3420, fits in any seq_t */
}

seq_t a(idx_t n)
{
    seq_t r = pow10_mod(n + 1);   /* 10^(n+1) mod 3420 */
    return r / 342;              /* integer division gives the digit */
}
