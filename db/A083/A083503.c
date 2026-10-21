/*
 * A083503  Basis of the n-th power arising in A083502.
 * https://oeis.org/A083503
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the least integer x > 1 such that xⁿ ≡ 1 (mod n)
 * Range:     u8 n <= 254, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* a(1) is defined as 3 */
    if (n == 1)
        return (seq_t)3;

    /* Find the smallest x > 1 with x^n ≡ 1 (mod n) */
    for (seq_t x = 2;; ++x)
    {
        if (mod_pow(x, (uint64_t)n, (seq_t)n) == (seq_t)1)
            return x;
    }
}
