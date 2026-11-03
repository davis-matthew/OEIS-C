/*
 * A075473  n^n + n! - 2*(n+1)^(n-1).
 * https://oeis.org/A075473
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^n + n! - 2*(n+1)^(n-1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 18 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

static seq_t fact_upto(idx_t m)
{
    seq_t r = 1;
    for (idx_t i = 2; i <= m; ++i)
        r *= (seq_t)i;
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0ULL)
        return 0;

    seq_t pow_nee  = ipow((seq_t)n,          (seq_t)n);
    seq_t fact     = fact_upto(n);
    seq_t pow_n1   = ipow((seq_t)(n + 1),    (seq_t)(n - 1));

    seq_t double_pow = pow_n1 + pow_n1;          /* 2 * (n+1)^(n-1) */
    seq_t result     = pow_nee + fact - double_pow;

    return result;
}
