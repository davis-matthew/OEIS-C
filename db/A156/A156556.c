/*
 * A156556  a(n) = Sum_{k=1..n} (n^k mod (n+k)).
 * https://oeis.org/A156556
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} (n^k mod (n + k))
 * Range:     u8 n <= 19, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    if (n == 0)
        return 0;
    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t mod = (seq_t)(n + k);
        seq_t term = mod_pow((seq_t)n, k, mod);
        sum += term;
    }
    return sum;
}
