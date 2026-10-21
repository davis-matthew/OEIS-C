/*
 * A062176  a(n) = 5^(n-1) mod n.
 * https://oeis.org/A062176
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5^(n-1) mod n
 * Range:     u8 n <= 284, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* Protect against zero input (though n ≥ 1 in the OEIS offset). */
    if (n == 0)
        return 0;

    /* Compute 5^(n-1) modulo n. */
    return mod_pow((seq_t)5, n - 1, (seq_t)n);
}
