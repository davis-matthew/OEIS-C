/*
 * A155485  a(n) = 5^n + (1 - 4^n)/3.
 * https://oeis.org/A155485
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^n + (1 - 4^n)/3 (OEIS A155485)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h> /* for idx_t definition in the build */

seq_t a(idx_t n)
{
    /* compute 5^n and 4^n with binary exponentiation */
    seq_t pow5 = 1;
    seq_t pow4 = 1;
    seq_t base5 = 5;
    seq_t base4 = 4;
    idx_t exp = n;

    while (exp > 0) {
        if (exp & 1) {
            pow5 *= base5;
            pow4 *= base4;
        }
        exp >>= 1;
        if (exp) {
            base5 *= base5;
            base4 *= base4;
        }
    }
    /* (4^n - 1) is divisible by 3 for all n ≥ 0 */
    seq_t tmp = (pow4 - 1) / 3;
    return pow5 - tmp;
}
