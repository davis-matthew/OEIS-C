/*
 * A220436  a(n) = A127546(n)^2.
 * https://oeis.org/A220436
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * (F(n)^4 + F(n+1)^4 + F(n+2)^4), where F(k) is the
 *            k‑th Fibonacci number
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   fib, fib_pair
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"
#include "../utils/fib_pair.h"

seq_t a(idx_t n)
{
    seq_t f0, f1, f2;

    /* Compute F(n) and F(n+1) in one call, then get F(n+2) = F(n) + F(n+1) */
    fib_pair(n, &f0, &f1);
    f2 = f0 + f1;

    /* Compute the fourth powers: (x)^4 = ((x)^2)^2 */
    seq_t f0_sq = f0 * f0;
    seq_t f0_4 = f0_sq * f0_sq;

    seq_t f1_sq = f1 * f1;
    seq_t f1_4 = f1_sq * f1_sq;

    seq_t f2_sq = f2 * f2;
    seq_t f2_4 = f2_sq * f2_sq;

    /* Sum the fourth powers and multiply by 2 */
    seq_t sum = f0_4 + f1_4 + f2_4;
    return 2 * sum;
}
