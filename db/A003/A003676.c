/*
 * A003676  Decimal expansion of the Planck constant h (Joule * second).
 * https://oeis.org/A003676
 *
 * Offset:    -33    (a(-33) is the first term)
 * Keywords:  nonn, cons, fini, full
 * Formula:   h = 662607015 * 10^(-42) J*s exactly (SI definition since 2019);
 *            a(n) = floor(662607015 / 10^(-25-n)) mod 10 for -33 <= n <= -25,
 *            and 0 after that (the expansion terminates)
 * Range:     u8 n <= -25, u16 n <= -25, u32 n <= -25, u64 n <= -25,
 *            u128 n <= -25, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    const uint64_t H = 662607015ULL; /* exact defining mantissa of h */
    idx_t k = n + 33;                /* 0-based digit position */
    uint64_t p = 1;
    idx_t i;
    if (k < 0 || k > 8) return (seq_t)0;
    for (i = 0; i < 8 - k; i++) p *= 10;
    return (seq_t)((H / p) % 10);
}
