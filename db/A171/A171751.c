/*
 * A171751  Maximum k with 1 <= k <= n such that nk + 1 is prime (or 0 if no
 *          such prime exists).
 * https://oeis.org/A171751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max{ k : 1 <= k <= n and n*k + 1 is prime }
 * Range:     u8 n <= 15, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    for (idx_t k = n; k > 0; --k) {
        seq_t v = (seq_t)n * k + 1;
        if (isPrimeGeneric(v))
            return (seq_t)k;
    }
    return 0;
}
