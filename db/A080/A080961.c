/*
 * A080961  Fourth binomial transform of A010686 (period 2: repeat 1,5).
 * https://oeis.org/A080961
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*5^n - 2*3^n, a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 54,
 *            big any n
 * Validated: standard, pass, 40 terms through n = 977 (2026-11-01)
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow3.h"

static inline seq_t power5(idx_t exp)
{
    seq_t result = 1;
    seq_t base   = 5;
    while (exp)
    {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t p5 = power5(n);
    seq_t p3 = pow3(n);
    seq_t res = p5 * 3;
    res -= p3 * 2;
    return res;
}
