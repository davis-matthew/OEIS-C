/*
 * A081516  Final term in n-th row of A081517.
 * https://oeis.org/A081516
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n >= 2, a(n) = p - n*(n-1)/2 where p is the least
 *            prime >= n*(n+1)/2 (the final term of row n of A081517)
 * Range:     u8 n <= 21, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Validated: partial, pass, 68 terms through n = 68 (2026-11-07)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"

seq_t a(idx_t n)
{
    if (n <= (idx_t)1)
        return (seq_t)1;

    seq_t prev = (seq_t)n * ((seq_t)n - 1) / 2;
    seq_t m = (seq_t)n * ((seq_t)n + 1) / 2;
    while (!is_prime_u64((uint64_t)m))
        ++m;
    return m - prev;
}
