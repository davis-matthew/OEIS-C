/*
 * A020782  Expansion of g.f. 1/((1-7*x)*(1-8*x)*(1-9*x)).
 * https://oeis.org/A020782
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7^(n+2) + 9^(n+2) - 2*8^(n+2)) / 2
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t pow7 = 1;   /* 7^(0) */
    seq_t pow8 = 1;   /* 8^(0) */
    seq_t pow9 = 1;   /* 9^(0) */

    /* compute 7^(n+2), 8^(n+2) and 9^(n+2) by repeated small‑int multiplication */
    for (idx_t i = 0; i < n + 2; ++i) {
        pow7 = pow7 * (seq_t)7;
        pow8 = pow8 * (seq_t)8;
        pow9 = pow9 * (seq_t)9;
    }

    /* numerator = 7^(n+2) + 9^(n+2) - 2*8^(n+2)  (always non‑negative) */
    seq_t sum  = pow7 + pow9;
    seq_t temp = pow8 << 1;      /* multiply by 2 */
    seq_t num  = sum - temp;

    /* divide by 2 */
    return num >> 1;
}
