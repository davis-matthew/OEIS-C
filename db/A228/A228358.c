/*
 * A228358  Numbers k such that 3*k - 4 is not prime.
 * https://oeis.org/A228358
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the n-th integer k>0 such that 3*k - 4 is not prime (offset 1) */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    /* The sequence is defined by searching for the successive integers k
       for which 3*k-4 is NOT prime.  We simply iterate k until the nth
       such integer is found.  The algorithm runs in O(n) time, which
       is more than fast enough for the problem limits.          */
    idx_t count = 0;   /* number of valid terms found so far   */
    idx_t k = 0;       /* current candidate integer            */

    while (count < n)
    {
        ++k;
        seq_t val = (seq_t)k * (seq_t)3 - (seq_t)4;
        if (!isPrimeGeneric(val))
        {
            ++count;
            if (count == n)
                return (seq_t)k;   /* found the n-th term */
        }
    }
    /* Control never reaches here for n >= 1 */
    return 0;
}
