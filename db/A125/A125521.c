/*
 * A125521  a(n) is the minimal difference between two distinct n-digit
 *          numbers with property that when one of them is typed into a
 *          calculator and rotated 180 degrees, the other one is seen.
 * https://oeis.org/A125521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(1)=3, a(2)=6, a(n)=10*a(n-2) for n>2
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 19, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This uses the closed‑form a(n) = (n odd?3:6) * 10^((n-1)/2)  */
#include "../utils/pow10.h"

seq_t a(idx_t n) {
    if (n == 1) return 3;
    if (n == 2) return 6;
    /* exponent for 10 */
    idx_t e = (n - 1) / 2;
    seq_t pow = pow10(e);
    seq_t base = (n & 1) ? 3 : 6;
    return base * pow;
}
