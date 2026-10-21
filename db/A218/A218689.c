/*
 * A218689  Sum_{k=0..n} C(n,k)^6*C(n+k,k)^6.
 * https://oeis.org/A218689
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} C(n,k)^6 * C(n+k,k)^6
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 5, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline seq_t pow6(seq_t x)
{
    seq_t r = x;
    for (int i = 1; i < 6; ++i)
        r *= x;
    return r;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t c1 = 1;                 /* C(n,0)   */
    seq_t c2 = 1;                 /* C(n+0,0) */

    for (idx_t k = 0; ; ++k)
    {
        seq_t t1 = pow6(c1);
        seq_t t2 = pow6(c2);
        sum += t1 * t2;

        if (k == n)
            break;

        seq_t num1 = (seq_t)(n - k);
        seq_t num2 = (seq_t)(n + k + 1);
        seq_t denom = (seq_t)(k + 1);

        c1 = (c1 * num1) / denom;
        c2 = (c2 * num2) / denom;
    }

    return sum;
}
