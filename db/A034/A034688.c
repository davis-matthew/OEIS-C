/*
 * A034688  Expansion of (1-25*x)^(-1/5), related to quintic factorial numbers
 *          A008548.
 * https://oeis.org/A034688
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (-25)^n * binomial(-1/5, n) Recurrence: a(0) = 1 a(n) =
 *            (25*n - 20)/n * a(n-1) for n >= 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t mult = ((seq_t)25 * i) - 20;
        result = (result * mult) / i;
    }
    return result;
}
