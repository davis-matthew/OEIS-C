/*
 * A021851  Decimal expansion of 1/847.
 * https://oeis.org/A021851
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor((10^(n+1))/847) mod 10
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent to the nth digit of the decimal expansion of 1/847 (0‑indexed). */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute 10^(n+1) modulo (847*10) = 8470, then
       integer‑divide by 847 to obtain the next decimal digit. */

    const uint64_t mod = 8470ULL;
    const uint64_t divisor = 847ULL;

    uint64_t exp  = (uint64_t)n + 1ULL;
    uint64_t base = 10ULL;
    uint64_t res  = 1ULL % mod;

    while (exp > 0)
    {
        if (exp & 1ULL)
            res = (res * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }

    uint64_t digit = res / divisor;
    return (seq_t)digit;
}
