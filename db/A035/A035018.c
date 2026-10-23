/*
 * A035018  One fifth of 9-factorial numbers.
 * https://oeis.org/A035018
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   5*a(n) = ∏_{j=1..n} (9*j-4) (with a(1)=1) Recurrence: a(1)=1,
 *            a(n) = (9*n-4) * a(n-1) for n>1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* offset starts at 1 */

    seq_t result = 1;
    for (idx_t j = 2; j <= n; ++j) {
        /* compute factor (9*j-4). Use seq_t on right‑hand side to avoid
           overflow of idx_t when n is large. */
        seq_t factor = (seq_t)(9 * (seq_t)j - 4);
        result *= factor;
    }
    return result;
}
