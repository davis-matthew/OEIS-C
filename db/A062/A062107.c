/*
 * A062107  Diagonal of table A062104.
 * https://oeis.org/A062107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*3^(n-4) for n ≥ 4 * with a(1) = 0, a(2) = 1, a(3) = 3
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 42, u128 n <= 82,
 *            big any n
 * Helpers:   pow3_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3_idx.h"

seq_t a(idx_t n)
{
    if (n < 4)
    {
        /* explicit handling of the first three terms */
        switch (n)
        {
            case 1: return (seq_t)0;
            case 2: return (seq_t)1;
            case 3: return (seq_t)3;
            default: return (seq_t)0;   /* never reached – defensive */
        }
    }

    /* a(n) = 10 * 3^(n-4) for n ≥ 4 */
    seq_t pow_part = pow3_idx(n - 4);
    return (seq_t)10 * pow_part;
}
