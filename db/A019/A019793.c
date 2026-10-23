/*
 * A019793  Expansion of 1/((1-5x)(1-6x)(1-9x)).
 * https://oeis.org/A019793
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 20*a(n-1) - 129*a(n-2) + 270*a(n-3); a(0)=1, a(1)=20,
 *            a(2)=271
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>


seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)20;
    if (n == 2) return (seq_t)271;

    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)20;
    seq_t a2 = (seq_t)271;
    seq_t a3;

    for (idx_t i = 3; i <= n; ++i) {
        a3 = 20 * a2 - 129 * a1 + 270 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
    }
    return a2;
}
