/*
 * A225831  Number of binary pattern classes in the (7,n)-rectangular grid:
 *          two patterns are in the same class if one of them can be obtained
 *          by a reflection or 180-degree rotation of the other.
 * https://oeis.org/A225831
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * If n == 0: a(n) = 1 * If n == 1: a(n) = 72 * If n == 2: a(n) =
 *            4224 * For n >= 3: * If n is even: a(n) = 2^(7n/2-1) * (
 *            2^(7n/2-1) + 2^(n/2-1) + 1 ) * If n is odd : a(n) =
 *            2^((7n-1)/2-1) * ( 2^((7n-1)/2) + 2^((n-1)/2) + 9 )
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 18,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)72;
    if (n == 2) return (seq_t)4224;

    if ((n & 1) == 0) {          /* n is even */
        idx_t expE = (7 * n) / 2 - 1;
        seq_t outer = pow2(expE);
        seq_t inner = outer + pow2(n / 2 - 1) + (seq_t)1;
        return outer * inner;
    } else {                     /* n is odd */
        idx_t expE = ((7 * n) - 1) / 2 - 1;
        seq_t outer = pow2(expE);
        seq_t high  = pow2(expE + 1);                 /* (7n-1)/2 */
        seq_t inner = high + pow2((n - 1) / 2) + (seq_t)9;
        return outer * inner;
    }
}
