/*
 * A078263  Product of the forward and reverse concatenations of 1 to n.
 * https://oeis.org/A078263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = A000422(n) * A007908(n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Validated: partial, pass, 14 terms through n = 14 (2026-11-01)
 * Helpers:   num_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/num_digits_idx.h"

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
    if (n < 1)
        return (seq_t)0;   /* values for n < 1 are undefined */

    seq_t f = 0;      /* forward concatenation */
    seq_t r = 0;      /* reverse concatenation */

    /* build forward concatenation 1..n */
    for (idx_t i = 1; i <= n; ++i) {
        idx_t di = num_digits_idx(i);
        f = f * pow10(di) + i;
    }

    /* build reverse concatenation by appending numbers from n down to 1 */
    for (idx_t i = n; i > 0; --i) {
        idx_t di = num_digits_idx(i);
        r = r * pow10(di) + i;
    }

    return f * r;
}
