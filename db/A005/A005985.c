/*
 * A005985  Length of longest trail (i.e., path with all distinct edges) on
 *          the edges of an n-cube.
 * https://oeis.org/A005985
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = n*2^(n-1) for even n; a(n) = (n-1)*2^(n-1) + 1 for odd n;
 *            a(0) = 0
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 59, u128 n <= 122,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t p;
    idx_t i;

    if (n == 0) {
        return (seq_t)0;
    }

    p = (seq_t)1;
    for (i = 1; i < n; i++) {
        p *= 2;
    }

    if (n % 2 == 0) {
        return (seq_t)n * p;
    }
    return ((seq_t)n - 1) * p + 1;
}
