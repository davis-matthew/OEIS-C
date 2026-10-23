/*
 * A021776  Decimal expansion of 1/772.
 * https://oeis.org/A021776
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1)/772) mod 10
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n-th decimal digit of 1/772 (zero‑based). */

#include <stdint.h>

static uint64_t mod_pow10(idx_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    uint64_t base   = 10 % mod;
    while (exp > 0)
    {
        if (exp & 1)
            result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Compute 10^(n+1) mod 7720. 7720 = 772 * 10. */
    uint64_t val = mod_pow10(n + 1, 7720ULL);
    /* The desired digit is floor(val / 772). */
    return (seq_t)(val / 772ULL);
}
