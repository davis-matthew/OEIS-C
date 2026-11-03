/*
 * A151688  G.f.: Product_{n>=0} (1 + x^(2^n-1) + 2*x^(2^n)).
 * https://oeis.org/A151688
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = Σ_{k≥0} 2^(wt(n+k)-k) * binomial(wt(n+k), k)
 * Range:     u8 n <= 59, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 69 (2026-11-01)
 * Helpers:   binomial, popcount_idx, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/popcount_idx.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t result = 0;
    for (idx_t k = 0; ; ++k)
    {
        idx_t m = n + k;
        unsigned pop = popcount_idx(m);
        if (k > pop)
            break;
        seq_t b = binomial((idx_t)pop, k);
        seq_t p = pow2((idx_t)(pop - k));
        result += b * p;
    }
    return result;
}
