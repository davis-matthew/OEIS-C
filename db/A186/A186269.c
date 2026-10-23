/*
 * A186269  a(n) = Product_{k=0..n-1} A084057(k+1).
 * https://oeis.org/A186269
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{i=1..n} (2^{i-1} * L_i), where L_i are Lucas
 *            numbers (L_0 = 2, L_1 = 1). Recurrence: a(0) = 1, a(n) = a(n-1)
 *            * 2^{n-1} * L_n
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 12,
 *            big any n
 * Helpers:   pow2, seq_mul, seq_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_one.h"
#include "../utils/seq_mul.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t prod = seq_one();

    if (n == 0)
        return prod;

    seq_t two = pow2(1);           /* constant 2 */
    seq_t two_power = seq_one();   /* 2^0 initially */

    seq_t L_prevprev = two;           /* L_0 = 2 */
    seq_t L_prev = seq_one();          /* L_1 = 1 */

    for (idx_t i = 1; i <= n; ++i)
    {
        /* Multiply the running product by 2^{i-1} * L_i */
        seq_t factor = seq_mul(two_power, L_prev);
        prod = seq_mul(prod, factor);

        /* Prepare Lucas numbers for next iteration */
        seq_t L_curr = L_prev + L_prevprev;
        L_prevprev = L_prev;
        L_prev = L_curr;

        /* Increment two_power to 2^i */
        two_power = seq_mul(two_power, two);
    }

    return prod;
}
