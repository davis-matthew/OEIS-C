/*
 * A134006  a(n) = 1^n + 3^n + 5^n + 7^n.
 * https://oeis.org/A134006
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1^n + 3^n + 5^n + 7^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* keep separate powers of 3, 5 and 7; the power of 1 is always 1 */
    seq_t p3 = 1;
    seq_t p5 = 1;
    seq_t p7 = 1;

    /* for n = 0 the loop is skipped and all powers remain 1,
       so the result is 1 + 1 + 1 + 1 = 4 as required. */
    for (idx_t i = 1; i <= n; ++i) {
        p3 *= (seq_t)3;
        p5 *= (seq_t)5;
        p7 *= (seq_t)7;
    }

    /* sum 1^n + 3^n + 5^n + 7^n */
    return (seq_t)1 + p3 + p5 + p7;
}
