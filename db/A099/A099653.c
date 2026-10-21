/*
 * A099653  a(n) is the number of n-subsets (n=1,2,...,10) of the 10 decimal
 *          digits from which prime numbers can be constructed including all n
 *          distinct digits either with or without repetitions; a(n) <=
 *          binomial(10,n).
 * https://oeis.org/A099653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = binomial(10,n) - binomial(6,n) - binomial(4,n), with
 *            a(1)=5
 * Range:     u8 n <= 2, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* special case for n = 1, as the general formula gives 0 */
    if (n == 1) {
        return (seq_t)5;
    }

    /* For n > 10 the binomials are zero, so the result is 0 automatically. */
    return binom((idx_t)10, n) - binom((idx_t)6, n) - binom((idx_t)4, n);
}
