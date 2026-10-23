/*
 * A165970  a(n) = sqrt( superfactorial(4n) / factorial(2n) ).
 * https://oeis.org/A165970
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^n * Π_{k=1..2n} (2k-1)!
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 2, u128 n <= 3,
 *            big any n
 * Helpers:   pow2, seq_mul, seq_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_one.h"
#include "../utils/seq_mul.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0u) {
        return seq_one();
    }

    seq_t result  = seq_one();   /* product of odd factorials */
    seq_t oddFact = seq_one();   /* factorial of the current odd number */

    for (idx_t k = 1; k <= 2*n; ++k) {
        if (k != 1) {
            /* update oddFactor to (2k-2)! * (2k-1)! from previous (2k-3)! */
            idx_t even = 2*k - 2;  /* 2(k-1) */
            oddFact = seq_mul(oddFact, (seq_t)even);                     /* * (2k-2) */
            oddFact = seq_mul(oddFact, (seq_t)(2*k - 1));                 /* * (2k-1) */
        }
        result = seq_mul(result, oddFact);   /* multiply current odd factorial */
    }

    /* multiply by 2^n */
    seq_t pow2n = pow2(n);
    result = seq_mul(result, pow2n);

    return result;
}
