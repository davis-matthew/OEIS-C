/*
 * A087790  Partial sums of A085068.
 * https://oeis.org/A087790
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} A085068(k), where A085068(k) = number of
 *            steps of x -> (4/3)*ceiling(x) from k to reach an integer
 * Range:     u8 n <= 24, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Validated: partial, pass, 62 terms through n = 61 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static seq_t A085068_a(seq_t n)
{
    seq_t p = n;
    seq_t denom = 1;
    seq_t steps = 0;

    while (true) {
        steps++;
        seq_t ceil_val = (p + denom - 1) / denom;
        p = ceil_val * 4;
        if (p % 3 == 0) {
            return steps;
        }
        denom = 3;
    }
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 0; ; i++) {
        sum += A085068_a((seq_t)i);
        if (i == n) break;
    }
    return sum;
}
