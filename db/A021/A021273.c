/*
 * A021273  Decimal expansion of 1/269.
 * https://oeis.org/A021273
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor((10^(n+1) mod 2690) / 269)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is equivalent to the nth decimal digit of 1/269, with a(0)
   being the first digit after the decimal point. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute 10^(n+1) modulo 2690; 2690 < 2^16 so 32‑bit arithmetic is safe */
    const uint32_t MOD = 2690;
    uint32_t base   = 10 % MOD;
    uint32_t result = 1U;

    /* exponent e = n+1; n is uint64_t so the addition is safe for the test range */
    idx_t e = n + 1;

    while (e)
    {
        if (e & 1ULL)
            result = (uint32_t)((uint64_t)result * base % MOD);
        base = (uint32_t)((uint64_t)base * base % MOD);
        e >>= 1;
    }

    /* result = 10^(n+1) mod 2690  ->  result = 269*(a(n)) + r  (0 ≤ r < 269)
       thus a(n) = result / 269.  The quotient is a single decimal digit. */
    return (seq_t)(result / 269);
}
