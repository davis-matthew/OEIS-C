/*
 * A226730  a(n) = n! + (2*n-1)!/(n-1)!.
 * https://oeis.org/A226730
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! + (2*n-1)!/(n-1)!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Multiply numbers from 1 up to up_to (inclusive). */
static seq_t _factorial(idx_t up_to)
{
    seq_t res = 1;
    for (idx_t i = 1; i <= up_to; ++i)
        res *= (seq_t)i;
    return res;
}

/* OEIS sequence A226730. */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset 1, but safe for n==0 */
        return (seq_t)0;

    /* n! */
    seq_t fact = _factorial(n);

    /* (2*n-1)!/(n-1)! = product_{k=n}^{2*n-1} k */
    idx_t lim = 2 * n - 1;
    seq_t prod = 1;
    for (idx_t k = n; k <= lim; ++k)
        prod *= (seq_t)k;

    return fact + prod;
}
