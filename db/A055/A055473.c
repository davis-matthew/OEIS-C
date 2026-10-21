/*
 * A055473  Powers of ten written in base 2.
 * https://oeis.org/A055473
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = Interpret the binary representation of 10^n as a decimal
 *            number
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 11,
 *            big any n
 * Helpers:   power_of_ten
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/power_of_ten.h"

/* Main sequence function required by the exercise. */
seq_t a(idx_t n)
{
    seq_t val   = power_of_ten(n);     /* 10^n as a seq_t */
    seq_t out   = 0;                   /* result being built */
    seq_t power = 1;                   /* 10^0 initially */

    /* Process each bit of val: if the bit is 1, add the corresponding
       power of 10 to the result.  This interprets the binary
       representation of 10^n as decimal digits. */
    while (val)
    {
        if (val & (seq_t)1)
            out += power;
        power *= (seq_t)10;
        val >>= 1;
    }

    return out;
}
