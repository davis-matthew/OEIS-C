/*
 * A021167  Decimal expansion of 1/163.
 * https://oeis.org/A021167
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10 * (10^n mod 163) / 163)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   pow10_mod_163
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10_mod_163.h"
/* Computes the nth decimal digit of 1/163 (0‑based).  n may be any
   non‑negative idx_t value; no caching is required for performance.  */

seq_t a(idx_t n)
{
    /*  10^n mod 163   < 163   ⇒  10 * that < 1630  */
    unsigned r   = pow10_mod_163(n);
    seq_t digit  = (seq_t)((10u * r) / 163u);
    return digit;
}
