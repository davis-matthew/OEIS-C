/*
 * A083826  Smallest k such that k+ 9*(A000042(n-1)*{n, mod 10) is digit
 *          reversal of k. A000042(n) = (10^n-1)/9.,a(10n) = 0.
 * https://oeis.org/A083826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, obsc, uned
 * Formula:   a(n) = 0 if n % 10 == 0, 12 if n == 1, 10^(floor(n/10)+1) if n %
 *            10 == 1 and n > 1, 9 * 10^(floor(n/10)+1)if n % 10 == 9,
 *            10^(floor(n/10)+1) + (n % 10 + 1) otherwise
 * Range:     u8 n <= 18, u16 n <= 21, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Validated: partial, pass, 21 terms through n = 21 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

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
    /* number of full tens in n */
    idx_t tens = n / 10;
    /* exponent for 10^x */
    idx_t exp  = tens + 1;
    /* base = 10^exp */
    seq_t base = pow10(exp);

    idx_t rem = n % 10;
    if (rem == 0)
        return (seq_t)0;
    if (rem == 1) {
        if (n == 1)
            return (seq_t)12;
        return base;
    }
    if (rem == 9)
        return base * 9;
    /* rem is 2..8 */
    return base + (seq_t)(rem + 1);
}
