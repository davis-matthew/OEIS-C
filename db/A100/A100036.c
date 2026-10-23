/*
 * A100036  a(n) = smallest m such that A100035(m) = n.
 * https://oeis.org/A100036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = partial sums of the sequence * s(i) = 1 for i = 1,2,3 *
 *            s(4) = s(5) = 2 * s(i) = 2 for odd i ≥ 5 * s(i) = 2*i – 7 for
 *            even i ≥ 6 * thus a(n) = Σ_{i=1..n} s(i)
 * Range:     u8 n <= 24, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t val;
        if (i <= 3) {
            val = (seq_t)1;
        } else if (i == 4 || i == 5) {
            val = (seq_t)2;
        } else if ((i & (idx_t)1) == 0) {
            val = (seq_t)2 * (seq_t)i - (seq_t)7;
        } else {
            val = (seq_t)2;
        }
        sum += val;
    }
    return sum;
}
