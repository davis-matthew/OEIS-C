/*
 * A021546  Decimal expansion of 1/542.
 * https://oeis.org/A021546
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1) / 542) mod 10 (digit in position n of the
 *            decimal expansion of 1/542)
 * Range:     u8 n <= 2, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Validated: partial, pass, 99 terms through n = 98 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t powmod10_mod542(idx_t exp)
{
    /* Computes 10^exp modulo 542.  All intermediate values fit in 64 bits. */
    uint64_t rem = 1;
    uint64_t base = 10;
    const uint64_t mod = 542;
    while (exp > 0)
    {
        if (exp & 1)
            rem = (rem * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return (seq_t)rem;
}

seq_t a(idx_t n)
{
    /* The remainder after n decimal digits of 1/542. */
    seq_t rem = powmod10_mod542(n);
    /* Next digit */
    return (seq_t)((rem * 10) / 542);
}
