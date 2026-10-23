/*
 * A032518  Sum of the integer part of 10/3-th roots of integers less than n.
 * https://oeis.org/A032518
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1}^n floor(k^(3/10)), with a(0)=0
 * Range:     u8 n <= 0, u16 n <= 38, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) is defined as 0; for n>=1 the sum starts at k=1. */
#include <stdint.h>

/* integer power for small exponents */
static inline seq_t pow_u64(seq_t base, unsigned int exp)
{
    seq_t res = (seq_t)1;
    for (unsigned int i = 0; i < exp; ++i)
        res *= base;
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    seq_t sum = 0;
    seq_t t = 1;                         /* current floor value */
    seq_t next_th = pow_u64(t + 1, 10);   /* (t+1)^10 : threshold for next t */

    for (idx_t k = 1; k <= n; ++k) {
        seq_t k3 = (seq_t)k * k * k;      /* k^3 */
        while (k3 >= next_th) {
            ++t;
            next_th = pow_u64(t + 1, 10);
        }
        sum += t;
    }
    return sum;
}
