/*
 * A078830  Numbers having in binary representation exactly one binary
 *          substring representing a prime.
 * https://oeis.org/A078830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n-th smallest integer whose binary representation
 *            contains exactly one contiguous substring that is a prime
 *            number. The sequence consists of all powers of two, together
 *            with 2^k+1 whenever 2^k+1 is composite (except that 3 is always
 *            included)
 * Range:     u8 n <= 12, u16 n <= 27, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   include_plus_one, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"
#include "../utils/include_plus_one.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t k = 1; ; k++)
    {
        seq_t val = pow2(k);
        count++;
        if (count == n) return val;

        if (include_plus_one(k))
        {
            seq_t valp = val + (seq_t)1;
            count++;
            if (count == n) return valp;
        }
    }
}
