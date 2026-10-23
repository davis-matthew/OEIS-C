/*
 * A100627  a(n) = 3^(2*prime(n) + 1).
 * https://oeis.org/A100627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3^(2*prime(n)+1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 12,
 *            big any n
 * Helpers:   is_prime, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1 according to OEIS.  For any
       other n the function returns 0. */
    if (n == 0)
        return (seq_t)0;

    idx_t prime = 0;
    idx_t count = 0;
    idx_t candidate = 2;

    while (count < n)
    {
        if (is_prime((uint64_t)candidate))
        {
            ++count;
            prime = candidate;
        }
        ++candidate;
    }

    /* exponent e = 2*prime + 1 */
    idx_t exp = 2 * prime + 1;
    return pow_seq((seq_t)3, exp);
}
