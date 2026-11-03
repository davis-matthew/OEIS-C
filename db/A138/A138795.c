/*
 * A138795  a(n) = (A138793(n+1)-A138793(n))/10^n.
 * https://oeis.org/A138795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = rev(n+1) * 10^max(0, n-9)
 * Range:     u8 n <= 10, u16 n <= 12, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Validated: partial, pass, 25 terms through n = 25 (2026-11-01)
 * Helpers:   reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/reverse_digits.h"

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
    seq_t inc = (seq_t)n + 1;            /* n+1 as seq_t */
    seq_t rev = reverse_digits(inc);     /* reverse decimal digits of n+1 */
    idx_t exp = (n > 9) ? (n - 9) : 0;   /* number of trailing zeros */
    seq_t pow = pow10(exp);              /* 10^exp */
    return rev * pow;                    /* final term */
}
