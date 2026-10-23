/*
 * A102491  Numbers whose base-20 representation can be written with decimal
 *          digits.
 * https://oeis.org/A102491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = Σ_{k≥0} ((n-1)/10^k mod 10) · 20^k (i.e. convert the
 *            decimal digits of n-1 into base 20)
 * Range:     u8 n <= 100, u16 n <= 8400, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* m = n-1 holds the decimal number we want to translate. */
    idx_t m = n - 1;

    seq_t result = 0;
    seq_t power = 1;          /* 20^k, starts with k = 0 */

    /* Extract decimal digits from least‑significant side. */
    while (m > 0) {
        unsigned digit = (unsigned)(m % 10);  /* digit is 0..9 */
        result += (seq_t)digit * power;
        m /= 10;
        power *= 20;                 /* multiply by 20 for next digit place */
    }

    return result;
}
