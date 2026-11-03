/*
 * A171869  a(n) is the period of A175555(n) in the sequence {A175555}.
 * https://oeis.org/A171869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 1 if n = 1; 2 if 2 ≤ n < 5; 2^(n-4) otherwise
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 35, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Validated: partial, pass, 36 terms through n = 36 (2026-11-01)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;
    if (n < 5)
        return (seq_t)2;
    return pow2(n - 4);
}
