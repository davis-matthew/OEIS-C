/*
 * A226029  First differences of A182402.
 * https://oeis.org/A226029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A182402(n+1) - A182402(n), where A182402(n) = total
 *            digits of the integers T(n-1)+1..T(n), T(k)=k(k+1)/2
 * Range:     u8 n <= 20, u16 n <= 1083, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 101 terms through n = 9516 (2026-11-07)
 * Helpers:   num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/num_digits.h"

static seq_t A226029_pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

static seq_t A182402_a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t low  = ((seq_t)(n - 1)) * ((seq_t)n) / 2;
    seq_t high = ((seq_t)n) * ((seq_t)n + 1) / 2;
    seq_t start = low + 1;

    seq_t ans = 0;
    int d_start = num_digits(start);
    int d_end   = num_digits(high);

    for (int d = d_start; d <= d_end; ++d)
    {
        seq_t pow_dm1 = A226029_pow10((idx_t)(d - 1));
        seq_t pow_d   = A226029_pow10((idx_t)d);

        seq_t first = start > pow_dm1 ? start : pow_dm1;
        seq_t last  = high < (pow_d - 1) ? high : (pow_d - 1);

        if (first > last)
            continue;

        seq_t count = last - first + 1;
        ans += count * (seq_t)d;
    }
    return ans;
}

seq_t a(idx_t n)
{
    return A182402_a(n + 1) - A182402_a(n);
}
