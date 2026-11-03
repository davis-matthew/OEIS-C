/*
 * A156556  a(n) = Sum_{k=1..n} (n^k mod (n+k)).
 * https://oeis.org/A156556
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} (n^k mod (n + k))
 * Range:     u8 n <= 19, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Validated: partial, pass, 53 terms through n = 52 (2026-11-01)
 * Helpers:   powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/powmod.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    if (n == 0)
        return 0;
    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t mod = (seq_t)(n + k);
        seq_t term = powmod((seq_t)n, k, mod);
        sum += term;
    }
    return sum;
}
