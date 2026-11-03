/*
 * A231601  Number of permutations of [n] avoiding ascents from odd to even
 *          numbers.
 * https://oeis.org/A231601
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil(n/2)! * ceil(n/2)^floor(n/2)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 14, u64 n <= 22, u128 n <= 36,
 *            big any n
 * Validated: standard, pass, 40 terms through n = 241 (2026-11-01)
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
    /* ceil(n/2) and floor(n/2) */
    idx_t m = (n + 1) / 2;   /* m = ceil(n/2) */
    idx_t k = n / 2;         /* k = floor(n/2) */

    seq_t result = fact_upto(m);          /* m! */

    if (k > 0) {
        seq_t powv = ipow((seq_t)m, (seq_t)k);  /* m^k */
        result *= powv;
    }

    return result;
}
