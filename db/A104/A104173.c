/*
 * A104173  a(n) is the smallest integer equal to the sum and the product of
 *          the same n positive integers: a(n) = i(1) + i(2) + ... + i(n) =
 *          i(1)*i(2)*...*i(n).
 * https://oeis.org/A104173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the smallest positive integer that can be expressed as
 *            the sum and the product of the same n positive integers
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   search_factors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/search_factors.h"

seq_t a(idx_t n)
{
    /* n >= 1 by definition. */
    if (n == 1)
        return (seq_t)1;              /* the only possibility is {1} */

    /* Search k from n upwards (lower bound) up to the known upper bound 2n. */
    for (idx_t i = n; i <= 2 * n; ++i) {
        seq_t k = (seq_t)i;
        if (search_factors(k, k, 2, n, 0, 0))
            return k;                /* minimal k found */
    }

    /* Theoretical guarantee a(n) <= 2n ensures we never reach here. */
    return (seq_t)0; /* placeholder; should not happen */
}
