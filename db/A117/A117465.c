/*
 * A117465  Denominator of -16/((n+2)*n*(n-2)*(n-4)).
 * https://oeis.org/A117465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn, easy
 * Formula:   a(n) = denominator of -16 / ((n+2)*n*(n-2)*(n-4))
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 257, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(2) = a(4) = 0 because the denominator is 0.               */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Handle the obvious zero denominators for n = 2 or 4. */
    if (n == 2 || n == 4 || n == 0)
        return 0;

    /* Use signed 64‑bit arithmetic;  n ≤ 10 000 ⇒ |product| < 10¹⁶ < 2⁶³. */
    int64_t a = (int64_t)n - 4;
    int64_t b = (int64_t)n - 2;
    int64_t c = (int64_t)n;
    int64_t d = (int64_t)n + 2;

    int64_t prod = a * b;
    prod = prod * c;
    prod = prod * d;

    int64_t abs_prod = prod >= 0 ? prod : -prod;

    /* Convert to seq_t; seq_t may be 8,16,32,64,128 bits or arbitrary precision. */
    seq_t denom = (seq_t)abs_prod;

    seq_t g = gcd((seq_t)16, denom);
    return denom / g;
}
