/*
 * A047232  Smallest k > n where n is a 4th power modulo k.
 * https://oeis.org/A047232
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = smallest k > n such that n is a 4th‑power residue modulo
 *            k (i.e. ∃x: x⁴ ≡ n (mod k))
 * Range:     u8 n <= 238, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_fourth_power_residue
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_fourth_power_residue.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for all n ≥ 0.  For n = 0 the answer is 1. */
    if (n == 0)
        return 1;

    for (idx_t k = n + 1; ; ++k) {
        if (is_fourth_power_residue(n, k))
            return (seq_t)k;
    }
}
