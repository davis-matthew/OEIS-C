/*
 * A010792  a(n) = n!*(n+3)! / 3!.
 * https://oeis.org/A010792
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * (n+3)! / 3! * Recurrence: a(0) = 1, * a(n) = a(n-1)
 *            * n * (n+3) for n >= 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t res = 1;
    for (idx_t k = 1; k <= n; ++k) {
        res *= (seq_t)k;
        res *= (seq_t)(k + 3);
    }
    return res;
}
