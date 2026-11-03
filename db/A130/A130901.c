/*
 * A130901  a(n) = Sum_{k=1..9} floor(10^n / k).
 * https://oeis.org/A130901
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..9} floor(10^n / k)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 100 (2026-11-01)
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
    seq_t power = pow10(n);
    seq_t sum = 0;
    static const unsigned k[9] = {1,2,3,4,5,6,7,8,9};
    for (int i = 0; i < 9; ++i)
        sum += power / k[i];
    return sum;
}
