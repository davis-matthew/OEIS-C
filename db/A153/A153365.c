/*
 * A153365  Number of zig-zag paths from top to bottom of a rectangle of width
 *          9 with 2*n rows whose color is that of the top right corner.
 * https://oeis.org/A153365
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=8, a(2)=28, a(n)=5*a(n-1)-5*a(n-2) for n>=3
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
seq_t a(idx_t n) {
    if (n == 0) return 0;            /* undefined behavior below offset, return 0 */
    if (n == 1) return (seq_t)8;
    if (n == 2) return (seq_t)28;
    seq_t prev2 = (seq_t)8;          /* a(1) */
    seq_t prev1 = (seq_t)28;         /* a(2) */
    for (idx_t i = 3; i <= n; ++i) {
        seq_t cur = (seq_t)5 * prev1 - (seq_t)5 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
