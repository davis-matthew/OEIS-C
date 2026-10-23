/*
 * A190531  Number of idempotents in Identity Difference Partial
 *          Transformation semigroup.
 * https://oeis.org/A190531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)*3^(n-2) + n*2^(n-1) - n + 2
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* not part of the sequence, safety */
    if (n == 1)
        return (seq_t)2;          /* special case: 3^(−1) is not needed */

    /* use seq_t arithmetic throughout */
    seq_t n_seq = (seq_t)n;
    seq_t term1 = (seq_t)(n - 1) * pow3(n - 2);      /* (n-1)*3^(n-2) */
    seq_t term2 = n_seq * pow2(n - 1);               /* n*2^(n-1) */

    /* a(n) = term1 + term2 - n + 2 */
    seq_t result = term1 + term2 - n_seq + (seq_t)2;
    return result;
}
