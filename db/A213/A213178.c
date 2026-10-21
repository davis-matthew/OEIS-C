/*
 * A213178  Total cell count of the expansion of a single cell, utilizing
 *          S1/B1 Game of Life cellular automata rules.
 * https://oeis.org/A213178
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(2*popcount(n)+1) for odd n; a(n) = a(n+1)/8 for even
 *            n; a(0) = 1
 * Range:     u8 n <= 13, u16 n <= 253, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    if (n & 1ULL) {               /* odd index */
        idx_t pc = __builtin_popcountll((unsigned long long)n);
        idx_t exp = 2 * pc + 1;
        return pow2(exp);
    } else {                      /* even index */
        idx_t pc = __builtin_popcountll((unsigned long long)(n + 1));
        idx_t exp = 2 * pc + 1;
        seq_t val = pow2(exp);
        return val / (seq_t)8;
    }
}
