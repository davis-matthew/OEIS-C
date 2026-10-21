/*
 * A224482  Numbers n for which the Lucas numbers modulo n is nondefective
 *          (residue complete).
 * https://oeis.org/A224482
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2,3,4,6,7,9,14,3^(n-5) for n >= 8 (OEIS A224482: Numbers
 *            n for which the Lucas numbers modulo n is nondefective)
 * Range:     u8 n <= 10, u16 n <= 15, u32 n <= 25, u64 n <= 45, u128 n <= 85,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* Handle the first 7 terms explicitly */
    switch (n)
    {
    case 1: return (seq_t)2;
    case 2: return (seq_t)3;
    case 3: return (seq_t)4;
    case 4: return (seq_t)6;
    case 5: return (seq_t)7;
    case 6: return (seq_t)9;
    case 7: return (seq_t)14;
    default: /* n >= 8 */
        return pow3(n - 5);
    }
}
