/*
 * A075105  Numerator of n/floor(log_2(n)); denominator is A075106(n).
 * https://oeis.org/A075105
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 91 terms through n = 9947 (2026-11-01)
 * Helpers:   gcd, ilog2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A075105 – numerator of n / ⌊log₂(n)⌋
   Formula: a(n) = n / gcd(n, ⌊log₂(n)⌋)   (n ≥ 2) */

#include <stddef.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/ilog2.h"

seq_t a(idx_t n)
{
    /* Strictly for n ≥ 2.  Below the offset the value is irrelevant. */
    if (n < 2) return 0;

    unsigned m = ilog2(n);          /* ⌊log₂(n)⌋ */
    seq_t g = gcd((seq_t)n, (seq_t)m);   /* gcd(n, ⌊log₂(n)⌋) */
    return (seq_t)n / g;                        /* numerator of reduced fraction */
}
