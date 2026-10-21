/*
 * A054138  T(n,4), array T as in A054134.
 * https://oeis.org/A054138
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 28, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A054138:  a(n) = 2*a(n-1) + binom(n+2,6),  a(4)=2, a(5)=10 */
#include <stdint.h>

/* multiplicative binomial coefficient C(n,6) using integer division
   to keep intermediate values small.  n is at least 6. */
static seq_t binom6(idx_t n)
{
    seq_t r = 1;
    for (idx_t i = 1; i <= 6; ++i)
    {
        /* multiply by (n - i + 1) then divide by i */
        r = r * (seq_t)(n - i + 1) / (idx_t)i;
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n < 4)
        return (seq_t)0;          /* value does not matter for offset <4 */

    if (n == 4)
        return (seq_t)2;

    if (n == 5)
        return (seq_t)10;

    seq_t val = (seq_t)10;          /* a(5) */
    for (idx_t i = 6; i <= n; ++i)
    {
        seq_t term = binom6(i + 2); /* C(i+2,6) = C(i+2, i-4) */
        val = (seq_t)2 * val + term;
    }
    return val;
}
