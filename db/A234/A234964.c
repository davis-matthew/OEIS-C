/*
 * A234964  Numbers of the form 123...n - (n+1).
 * https://oeis.org/A234964
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concat(1,2,…,n) – (n+1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   digits, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"
#include "../utils/digits.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                /* offset not defined for n < 2 */

    seq_t value = 0;

    for (idx_t k = 1; k <= n; ++k) {
        int d = digits(k);                     /* decimal digit count of k */
        value = value * pow10((idx_t)d) + (seq_t)k;
    }

    return value - ((seq_t)n + 1);
}
