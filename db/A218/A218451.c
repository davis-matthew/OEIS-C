/*
 * A218451  a(n) = 10^n minus its binary weight.
 * https://oeis.org/A218451
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = 10^n - popcount(10^n) (OEIS A218451)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 972 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}
#include <stddef.h>

seq_t a(idx_t n)
{
    seq_t val = pow10(n);          /* 10^n */
    seq_t x   = val;              /* make a copy for bit‑testing */
    size_t pop = 0;                /* binary weight */

    while (x) {
        pop += (size_t)(x & 1);    /* add least‑significant bit */
        x >>= 1;                   /* shift right by one bit */
    }

    return val - (seq_t)pop;       /* subtract the popcount */
}
