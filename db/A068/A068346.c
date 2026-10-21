/*
 * A068346  a(n) = n'' = second arithmetic derivative of n.
 * https://oeis.org/A068346
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 3071, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Compute the second arithmetic derivative:
 *  a(n) = D(D(n)),  D is the arithmetic derivative
 *  defined by D(p)=1 for prime p,  D(mn)=m D(n)+n D(m),
 *  and D(1)=0.  This implements: a(n)=arith_derivative(arith_derivative(n)). */

#include <stdbool.h>

static seq_t arithmetic_derivative(idx_t n)
{
    if (n < 2) return 0;
    seq_t sum = 0;
    idx_t tmp = n;
    for (idx_t p = 2; p * p <= tmp; ++p)
    {
        if (tmp % p == 0)
        {
            unsigned exp = 0;
            while (tmp % p == 0)
            {
                tmp /= p;
                ++exp;
            }
            /* n * exp / p is always an integer */
            sum += (seq_t)n * (seq_t)exp / (seq_t)p;
        }
    }
    if (tmp > 1)                  /* remaining prime factor */
        sum += (seq_t)n / (seq_t)tmp;
    return sum;
}

seq_t a(idx_t n)
{
    seq_t first = arithmetic_derivative(n);
    if (first < 2) return 0;
    /* first fits in idx_t for the sizes used in the tests (n up to ~10⁴) */
    return arithmetic_derivative((idx_t)first);
}
