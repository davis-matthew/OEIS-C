/*
 * A156275  a(n) = 10^n*Catalan(n).
 * https://oeis.org/A156275
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10^n * Catalan(n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 13, u128 n <= 25,
 *            big any n
 * Validated: standard, pass, 33 terms through n = 199 (2026-11-01)
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/catalan.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

seq_t a(idx_t n)
{
    return catalan(n) * pow10(n);
}
