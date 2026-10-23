/*
 * A172507  a(n) = numerator of fraction a/b, where gcd(a, b) = 1, whose
 *          decimal representation has the form (n).(n).
 * https://oeis.org/A172507
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * (10^len(n)+1) / gcd(n, 10^len(n))
 * Range:     u8 n <= 10, u16 n <= 30, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    /* number of decimal digits of n */
    idx_t len = 0;
    idx_t t = n;
    do {
        len++;
        t /= 10;
    } while (t);

    /* compute 10^len */
    seq_t pow10 = 1;
    for (idx_t i = 0; i < len; ++i) {
        pow10 *= (seq_t)10;
    }

    /* reduce the fraction */
    seq_t g = gcd((seq_t)n, pow10);
    seq_t n_div_g = (seq_t)n / g;
    seq_t result = n_div_g * (pow10 + (seq_t)1);
    return result;
}
