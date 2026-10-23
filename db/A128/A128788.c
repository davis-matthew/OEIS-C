/*
 * A128788  a(n) = n^2*9^n.
 * https://oeis.org/A128788
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 * 9^n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;

    /* Compute 9^n using binary exponentiation */
    seq_t pow9 = 1;
    seq_t base  = 9;
    idx_t exp = n;
    while (exp) {
        if (exp & 1U)
            pow9 = pow9 * base;
        base = base * base;
        exp >>= 1;
    }

    /* Compute n^2 */
    seq_t nn  = (seq_t)n;
    seq_t n2 = nn * nn;

    /* Return n^2 * 9^n */
    return n2 * pow9;
}
