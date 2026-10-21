/*
 * A222112  Initial step in Goodstein sequences: write n-1 in hereditary
 *          binary representation, then bump to base 3.
 * https://oeis.org/A222112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = evaluation of the hereditary binary representation of n-1
 *            in base 3
 * Range:     u8 n <= 16, u16 n <= 16, u32 n <= 16, u64 n <= 256,
 *            u128 n <= 256, big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow3.h"

/* Cache for values f(m) where m < 64 (exponent positions) */
static seq_t _hered_cache[64];
static bool _hered_done[64] = {0};

/* Recursive computation of f(m) = value of hereditary binary of m in base 3 */
static seq_t _hered(idx_t m)
{
    if (m < 64 && _hered_done[m])
        return _hered_cache[m];

    seq_t sum = 0;
    for (idx_t i = 0; i < 64; ++i) {
        if (m & ((idx_t)1 << i)) {
            seq_t fi = _hered(i);
            seq_t term = pow3((idx_t)fi);   /* 3^f(i) */
            sum += term;
        }
    }

    if (m < 64) {
        _hered_cache[m] = sum;
        _hered_done[m] = true;
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n <= 1)   /* offset 1: a(1) = 0 */
        return 0;

    idx_t m = n - 1;
    return _hered(m);
}
