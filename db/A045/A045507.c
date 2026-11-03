/*
 * A045507  Concatenate powers of 2.
 * https://oeis.org/A045507
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Validated: standard, pass, 31 terms through n = 80 (2026-11-01)
 * Helpers:   num_digits, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Concatenate powers of 2
   a(n) = 1 2 4 8 16 32 … concatenated up to 2^n.
   Recurrence: a(0) = 1,  a(n) = a(n‑1) * 10^digits(2^n) + 2^n
*/

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/num_digits.h"
#include "../utils/pow2.h"

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
    seq_t result = 0;

    for (idx_t i = 0; i <= n; ++i) {
        seq_t power   = pow2(i);                                 /* 2^i */
        seq_t pow10d  = pow10((idx_t)num_digits(power));         /* 10^digits(2^i) */
        result = result * pow10d + power;                       /* concatenate */
    }

    return result;
}
