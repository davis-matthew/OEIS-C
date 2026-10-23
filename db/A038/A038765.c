/*
 * A038765  Next-to-last diagonal of A024462.
 * https://oeis.org/A038765
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^(n-2)*(n+5) for n>0, a(0)=1, a(1)=2
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 38, u128 n <= 78,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    /* n >= 2: compute 3^(n-2) * (n+5) */
    seq_t val = pow3(n - 2);
    val *= (seq_t)(n + 5);
    return val;
}
