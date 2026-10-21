/*
 * A093002  Product of factorials of next n numbers.
 * https://oeis.org/A093002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=n}^{2n-2} k! (a(1)=1)
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   factorial, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/factorial.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* factorial of (n-1) */

    seq_t fact   = factorial(n - 1);    /* (n-1)!  */
    seq_t result = 1;                   /* product accumulator */

    /* multiply factorials from k=n up to k=2n-2 */
    for (idx_t k = n; k <= 2 * n - 2; ++k)
    {
        fact   = seq_mul(fact, (seq_t)k);    /* fact = k!  */
        result = seq_mul(result, fact);      /* result *= fact */
    }
    return result;
}
