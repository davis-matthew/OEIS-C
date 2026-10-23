/*
 * A121864  See Comments lines for definition.
 * https://oeis.org/A121864
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = interpret 2^n with bases 2^(n-1), 2^(n-2), …, 2^4 (i.e.
 *            16)
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 7, u64 n <= 8, u128 n <= 9,
 *            big any n
 * Helpers:   pow_seq, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq.h"
#include "../utils/seq_mul.h"

static seq_t interpret_in_base(seq_t value, seq_t base)
{
    seq_t result = 0;
    seq_t power  = 1;

    while (value > 0)
    {
        seq_t mod   = value % 10;          /* decimal digit */
        uint8_t d   = (uint8_t)mod;
        result += (seq_t)d * power;
        power = seq_mul(power, base);
        value /= 10;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n < 4)
        return 0;                          /* undefined below offset */

    seq_t value = pow_seq((seq_t)2, (int)n);   /* 2^n */

    for (idx_t k = n - 1; k >= 4; --k)
    {
        seq_t base = pow_seq((seq_t)2, (int)k); /* 2^k */
        value = interpret_in_base(value, base);
    }

    return value;
}
