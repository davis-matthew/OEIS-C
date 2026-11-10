/*
 * A099259  A100287(n+1) - A000960(n).
 * https://oeis.org/A099259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A100287(n+1) - A000960(n)
 * Range:     u8 n <= 15, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Validated: partial, pass, 77 terms through n = 77 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A100287_a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t val = (seq_t)n * (seq_t)n - (seq_t)n + 1;
    for (idx_t d = n - 1; d > 0; --d)
        val = (seq_t)d * ((val - 1) / d);
    return val;
}

static seq_t A000960_a(idx_t n)
{
    seq_t result = (seq_t)n * (seq_t)n;
    for (seq_t divisor = (seq_t)n - 1; divisor > 0; divisor--) {
        seq_t quotient = result / divisor;
        if (result % divisor == 0)
            quotient--;
        result = quotient * divisor;
    }
    return result;
}

seq_t a(idx_t n)
{
    return A100287_a(n + 1) - A000960_a(n);
}
