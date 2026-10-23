/*
 * A048355  a(n) is the index of the smallest triangular number containing
 *          exactly n 0's.
 * https://oeis.org/A048355
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 0 for n = 1 * a(n) = 24 for n = 2 * a(n) = 200 for n = 3
 *            * a(n) = 775 for n = 4 * a(n) = 2 * 10^((n+1)/2) for n odd ≥ 5 *
 *            a(n) = 10^((n+2)/2) for n even ≥ 6 * * This implements the
 *            formula given in the OEIS entry
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 17, u64 n <= 36, u128 n <= 75,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    switch (n) {
    case 1: return (seq_t)0;
    case 2: return (seq_t)24;
    case 3: return (seq_t)200;
    case 4: return (seq_t)775;
    default:
        /* n ≥ 5 */
        if (n & 1) {                    /* n odd */
            idx_t exp = (n + 1) / 2;
            seq_t val = pow10(exp);
            return val * (seq_t)2;
        } else {                        /* n even */
            idx_t exp = (n + 2) / 2;
            return pow10(exp);
        }
    }
}
