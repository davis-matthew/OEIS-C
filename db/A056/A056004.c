/*
 * A056004  Initial step in Goodstein sequences: write n in hereditary
 *          representation base 2, bump to base 3, then subtract 1.
 * https://oeis.org/A056004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [evaluate hereditary base‑2 representation of n with all
 *            2's replaced by 3's] – 1 (initial step in Goodstein sequences)
 * Range:     u8 n <= 15, u16 n <= 15, u32 n <= 15, u64 n <= 255,
 *            u128 n <= 255, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

static seq_t raw_eval(idx_t n)
{
    if (n < 2)                      /* base cases: 0 and 1 */
        return (seq_t)n;

    seq_t result = 0;
    idx_t bit = 0;
    idx_t temp = n;

    /* iterate over binary digits of n */
    while (temp > 0) {
        if (temp & 1) {
            seq_t exp = raw_eval(bit);           /* exponent in hereditary form */
            seq_t term = big_pow((seq_t)3, exp);  /* 3^exp */
            result += term;
        }
        bit++;
        temp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;        /* value undefined for n < 1; harmless guard */

    seq_t bumped = raw_eval(n);
    return bumped - 1;
}
