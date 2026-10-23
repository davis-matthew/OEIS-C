/*
 * A110462  Integers with mutual residues -8.
 * https://oeis.org/A110462
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=9, a(2)=17, a(3)=145, and for n>3 a(n)=a(n-1)^2 + 8*a(n-1)
 *            - 8
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)9;
    if (n == 2) return (seq_t)17;
    if (n == 3) return (seq_t)145;

    seq_t val = (seq_t)145;
    for (idx_t i = 4; i <= n; ++i) {
        val = val * val + (seq_t)8 * val - (seq_t)8;
    }
    return val;
}
