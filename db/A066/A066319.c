/*
 * A066319  A labeled structure simultaneously a tree and a cycle.
 * https://oeis.org/A066319
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^(n-2) * (n-1)! (for n ≥ 1, with a(1)=a(2)=1)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>          /* for size_t */

seq_t a(idx_t n)
{
    /* Handle the first two terms directly. */
    if (n <= 2)
        return (seq_t)1;

    /* Compute (n-1)! iteratively. */
    seq_t fact = (seq_t)1;
    for (idx_t i = 2; i < n; ++i)
        fact *= (seq_t)i;

    /* Compute n^(n-2) using exponentiation by squaring. */
    seq_t pow   = (seq_t)1;
    seq_t base  = (seq_t)n;
    idx_t exp   = n - 2;

    while (exp != 0) {
        if (exp & 1)
            pow *= base;
        base *= base;
        exp >>= 1;
    }

    /* Return the product of factorial and power. */
    return fact * pow;
}
