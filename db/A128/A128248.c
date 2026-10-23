/*
 * A128248  a(n) = Sum_{k=1..phi(n)} t(k,n)*(-1)^k, where t(k,n) is the k-th
 *          positive integer that is coprime to n and phi(n) = A000010(n).
 * https://oeis.org/A128248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=1..phi(n)} t(k,n)*(-1)^k, where t(k,n) is the *
 *            k-th integer >0 coprime with n. Coprime list is taken in
 *            increasing * order, the signs alternate starting with negative
 * Range:     i8 n <= 190, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)-1;          /* special case: a(1) = -1 */

    seq_t sum = 0;                         /* running sum */
    idx_t k = 0;                           /* index of current coprime */
    seq_t sn = (seq_t)n;                   /* cast once for gcd */

    for (idx_t i = 1; i <= n-1; ++i)
    {
        if (gcd((seq_t)i, sn) == (seq_t)1) /* i is coprime to n */
        {
            ++k;
            seq_t val = (seq_t)i;
            sum += (k & 1) ? -val : val;   /* alternation: (-1)^k */
        }
    }
    return sum;
}
