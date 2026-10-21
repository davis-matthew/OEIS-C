/*
 * A096141  a(n) = sum of n n-th powers starting from n^n.
 * https://oeis.org/A096141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{k=n}^{2n-1} k^n (offset 1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = n; k <= 2 * n - 1; ++k) {
        seq_t base = (seq_t)k;     /* promote k to seq_t */
        seq_t res  = 1;
        idx_t exp  = n;

        /* binary exponentiation: compute k^n */
        while (exp) {
            if (exp & 1U) res *= base;
            base *= base;
            exp >>= 1U;
        }
        sum += res;
    }
    return sum;
}
