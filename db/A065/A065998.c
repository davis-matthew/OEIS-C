/*
 * A065998  Concatenate n and number of divisors of n.
 * https://oeis.org/A065998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concat(n, d(n)) where d(n) is the number of divisors of n
 * Range:     u8 n <= 25, u16 n <= 655, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_count, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/digit_count.h"
#include "../utils/pow10.h"

/* Helper to count divisors of a 64‑bit integer. */
static seq_t divisor_count_u64(uint64_t m)
{
    uint64_t cnt = 0;
    for (uint64_t i = 1; i <= m / i; ++i)
    {
        if (m % i == 0)
        {
            if (i * i == m)
                cnt += 1;
            else
                cnt += 2;
        }
    }
    return (seq_t)cnt;
}

seq_t a(idx_t n)
{
    uint64_t nn = (uint64_t)n;                /* index as native integer */
    seq_t dn  = divisor_count_u64(nn);        /* number of divisors of n */

    unsigned d_digits = digit_count((uint64_t)dn); /* decimal digits of dn */
    seq_t mult   = pow10((idx_t)d_digits);    /* 10^d_digits as seq_t */

    return (seq_t)nn * mult + dn;
}
