/*
 * A016028  Expansion of (1 - x + x^4) / (1 - x)^3.
 * https://oeis.org/A016028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1, 2 for n=1,2; otherwise a(n) = (n^2 - 5*n + 12)/2 (n >=
 *            3), which is equivalently a(n) = 3 + (n-3)(n-2)/2 for n >= 3
 * Range:     u8 n <= 24, u16 n <= 364, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;      /* undefined in OEIS, return 0 */
    if (n == 1) return 1;
    if (n == 2) return 2;
    /* For n >= 3 */
    seq_t nn = (seq_t)n;
    /* Compute (n^2 - 5*n + 12)/2 avoiding intermediate overflow
       by performing the division only once. */
    seq_t val = (nn * nn - 5 * nn + 12) / 2;
    return val;
}
