/*
 * A182166  The total number of components of size 2 over all simple labeled
 *          graphs on n nodes.
 * https://oeis.org/A182166
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = C(n,2) * 2^C(n-2,2) (OEIS A182166)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 9, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow, binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 2.  For n < 2 the value is not
       defined by OEIS, but returning 0 satisfies the requirement that the
       function works for all idx_t values. */
    if (n < 2)
        return (seq_t)0;

    /* C(n,2) */
    seq_t comb2 = binom(n, (idx_t)2);

    /* C(n-2,2) -- binom(0,2) and binom(1,2) are handled correctly
       by the binom routine. */
    seq_t exp = binom(n - 2, (idx_t)2);

    /* 2^exp */
    seq_t pow2 = big_pow((seq_t)2, exp);

    /* Product */
    return comb2 * pow2;
}
