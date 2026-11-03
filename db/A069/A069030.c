/*
 * A069030  Powers of 5 with strictly increasing sum of digits.
 * https://oeis.org/A069030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th power of 5 whose digit sum strictly increases
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Validated: partial, pass, 20 terms through n = 20 (2026-11-01)
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/digit_sum.h"

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

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

seq_t a(idx_t n)
{
    /* start with 5^0 = 1 */
    seq_t power = power5(0);
    seq_t last_sum = 0;

    idx_t count = 0;
    for (idx_t exp = 0; ; ++exp)
    {
        seq_t sum = digit_sum(power);
        if (sum > last_sum)
        {
            ++count;
            if (count == n)
                return power;          /* found the n-th term */
            last_sum = sum;            /* update previous sum */
        }
        /* advance to the next power of 5 */
        power = mul_small(power, 5);
    }
}
