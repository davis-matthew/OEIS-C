/*
 * A078614  First differences of A072633.
 * https://oeis.org/A078614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A072633(n) - A072633(n-1), with A072633(n) = 1 +
 *            round(n/log(2)); so a(n) = round(n/log(2)) - round((n-1)/log(2))
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 76 terms through n = 995 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    static const long double inv_ln2 = 1.442695040888963407359924681001892137L;
    long double v1 = (long double)n * inv_ln2 + 0.5L;
    long double v0 = (long double)(n - 1) * inv_ln2 + 0.5L;
    uint64_t r1 = (uint64_t)v1;
    uint64_t r0 = (uint64_t)v0;
    return (seq_t)(r1 - r0);
}
