/*
 * A226091  Smallest m such that A225985(m) = A014261(n).
 * https://oeis.org/A226091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = t - (((d+1)/2)*5^(L-1) - 1), where t = A014261(n) has L
 *            digits and leading digit d (smallest m with A225985(m) = t is
 *            the count of numbers <= t having an odd digit)
 * Range:     u8 n <= 55, u16 n <= 2655, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 9643 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t t = 0;
    seq_t pow10 = 1;
    seq_t pow5 = 1;      /* 5^(L-1) after the loop */
    seq_t lead = 0;
    idx_t temp = n;
    int first = 1;

    while (temp > 0) {
        idx_t r = temp % 5;
        if (r == 0)
            r = 5;
        seq_t dg = (seq_t)(2 * r - 1);
        t += dg * pow10;
        lead = dg;
        if (!first)
            pow5 *= 5;
        first = 0;
        pow10 *= 10;
        temp = (temp - r) / 5;
    }

    seq_t evens = ((lead + 1) / 2) * pow5 - 1;
    return t - evens;
}
