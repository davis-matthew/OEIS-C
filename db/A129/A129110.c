/*
 * A129110  A transformation of the Catalan sequence.
 * https://oeis.org/A129110
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(2n) = 1, a(2n+1) = 1 + (-1)^n * Catalan(n+1)
 * Range:     i8 n <= 8, i16 n <= 16, i32 n <= 32, i64 n <= 45, i128 n <= 45,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*        where Catalan(k) is the k-th Catalan number.      */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    /* Even indices have value 1. */
    if ((n & 1) == 0)
        return (seq_t)1;

    /* Odd indices: n = 2k+1 -> k = (n-1)/2. */
    idx_t k = (n - 1) >> 1;

    /* Compute Catalan(k+1).  Use the provided utility function
       if available; its implementation is efficient and exact for
       all supported seq_t types. */
    seq_t cat = catalan(k + 1);

    /* Add 1 plus (-1)^k * Catalan(k+1).  (-1)^k is +1 for even k,
       -1 for odd k. */
    if ((k & 1) == 0)
        return ((seq_t)1 + cat);
    else
        return ((seq_t)1 - cat);
}
