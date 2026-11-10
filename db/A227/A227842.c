/*
 * A227842  First differences of A014817.
 * https://oeis.org/A227842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A014817(n+1) - A014817(n), where A014817(m) =
 *            Sum_{k=1..m} floor(k^2/m)
 * Range:     u8 n <= 14, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Validated: partial, pass, 80 terms through n = 80 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A014817_a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t sq = (seq_t)k * (seq_t)k;
        seq_t val = sq / (seq_t)n;
        sum += val;
    }
    return sum;
}

seq_t a(idx_t n)
{
    return A014817_a(n + 1) - A014817_a(n);
}
