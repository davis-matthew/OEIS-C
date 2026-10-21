/*
 * A053179  Numbers ending in 3 which are not prime.
 * https://oeis.org/A053179
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = nth composite number ending in digit 3 (computed by
 *            scanning candidates)
 * Range:     u8 n <= 12, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t candidate = 3;   /* first number ending with 3 */

    for (;; candidate += 10)
    {
        if (!isPrimeGeneric(candidate))
        {
            ++count;
            if (count == n)
                return candidate;
        }
    }
}
