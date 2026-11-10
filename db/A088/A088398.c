/*
 * A088398  a(n) = A069528(n)/n.
 * https://oeis.org/A088398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A069528(n)/n, where A069528(n) is the smallest multiple
 *            of n with digit sum 9 (0 if none). Computed by a search over
 *            multiples
 * Range:     u8 n <= 28, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Validated: partial, pass, 94 terms through n = 94 (2026-11-07)
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    if (n % 11 == 0 || n % 101 == 0 || n % 271 == 0 ||
        n % 999 == 0 || n % 4649 == 0)
        return 0;

    idx_t step;
    if (n % 9 == 0)
        step = 1;
    else if (n % 3 == 0)
        step = 3;
    else
        step = 9;

    idx_t k = step;
    while (1) {
        seq_t prod = (seq_t)n * (seq_t)k;
        if (digit_sum(prod) == 9)
            return (seq_t)k;
        k += step;
    }
}
