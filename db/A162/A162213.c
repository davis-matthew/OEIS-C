/*
 * A162213  a(n) = the smallest positive multiple of n with exactly n digits
 *          when written in binary.
 * https://oeis.org/A162213
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = n * ceil(2^(n-1) / n)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

/* 
 * Return the smallest positive multiple of n with exactly n binary digits.
 * Uses the definition a(n) = n * ceil(2^(n-1)/n). 
 */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                    /* not used, but safe */

    /* 2^(n-1) */
    seq_t val = pow2(n - 1);

    /* n as a seq_t */
    seq_t sn = (seq_t)n;

    /* r = val mod sn */
    seq_t r = val % sn;

    /* if val is already a multiple of sn, it has the correct length */
    if (r == 0)
        return val;

    /* otherwise add the needed amount to reach the next multiple */
    return val + (sn - r);
}
