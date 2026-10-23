/*
 * A122006  Expansion of x^2*(1-x)/((1-3*x)*(1-3*x^2)).
 * https://oeis.org/A122006
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=0, a(2)=1, a(3)=2; for n≥4 a(n) = 3*a(n-1) + 3*a(n-2) -
 *            9*a(n-3)
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 22, u64 n <= 42, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t pow3(idx_t e)
{
    seq_t r = (seq_t)1;
    for (idx_t i = 0; i < e; ++i)
        r *= (seq_t)3;
    return r;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)0;

    if ((n & 1ULL) == 0) /* even n */
        return pow3(n - 2);

    /* odd n (n ≥ 3) */
    return pow3(n - 2) - pow3((n - 3) / 2);
}
