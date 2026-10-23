/*
 * A166334  a(n) = (3*n)!/(2^n*n!).
 * https://oeis.org/A166334
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3*n)!/(2^n*n!) * Recurrence: a(0) = 1, * a(n) =
 *            3*(3*n-1)*(3*n-2)/(2) * a(n-1) for n ≥ 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    seq_t result = (seq_t)1;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t factor = (seq_t)3 * ((seq_t)(3 * k - 1));
        factor *= (seq_t)(3 * k - 2);
        factor /= (seq_t)2;
        result *= factor;
    }
    return result;
}
