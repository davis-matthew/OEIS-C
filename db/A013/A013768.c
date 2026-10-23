/*
 * A013768  a(n) = 21^(3*n + 1).
 * https://oeis.org/A013768
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 21^(3*n + 1)
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 2, u64 n <= 4, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Fast exponentiation for arbitrary seq_t types. */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp)
    {
        if (exp & (idx_t)1)
            result *= base;
        exp >>= (idx_t)1;
        if (exp)
            base *= base;
    }
    return result;
}

/* OEIS sequence A013768 */
seq_t a(idx_t n)
{
    idx_t exp = 3 * n + (idx_t)1;
    return pow_seq((seq_t)21, exp);
}
