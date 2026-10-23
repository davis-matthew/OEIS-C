/*
 * A101875  Duplicate of A101871.
 * https://oeis.org/A101875
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = product over prime divisors p of (2n+1) of
 *            partition(v_p(2n+1)), where partition(k) is the number of
 *            partitions of k. The sequence counts abelian groups of order
 *            4n+2 = 2 * (2n+1)
 * Range:     i8 n <= 104, i16 n <= 104, i32 n <= 104, i64 n <= 104,
 *            i128 n <= 104, big any n
 * Helpers:   partition_of_exp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation factors k = 2n+1 and multiplies the partition numbers
   of the prime‑exponents.  A small DP computes partition(k) for the very
   small exponents that can occur (k ≤ 2·10⁴ in the tests).  */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/partition_of_exp.h"

seq_t a(idx_t n)
{
    /* Compute k = 2n+1 in an enlarged type to avoid overflow of idx_t. */
    unsigned __int128 tmp = ((unsigned __int128)n) * 2 + 1;
    uint64_t k = (uint64_t)tmp;       /* k fits into 64 bits for the test range */

    seq_t result = 1;
    uint64_t temp = k;

    /* Trial‑division factorisation of k.  Since k ≤ 20001 for the tests,
       this is trivial and fast. */
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            idx_t exp = 0;
            do {
                temp /= p;
                ++exp;
            } while (temp % p == 0);
            result *= partition_of_exp(exp);
        }
    }

    /* Any remaining prime factor has exponent 1; partition(1) == 1,
       so it does not affect the product. */
    return result;
}
