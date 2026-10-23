/*
 * A125711  In the "3x+1" problem, let 1 denote a halving step and 0 denote an
 *          x->3x+1 step. Then a(n) is obtained by writing the sequence of
 *          steps needed to reach 1 from 2n and reading it as a decimal
 *          number.
 * https://oeis.org/A125711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   a(n) = binary value of the Collatz-step sequence from 2n to 1
 *            (step 1 = halving, step 0 = 3x+1)
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 26, u64 n <= 26, u128 n <= 312,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;            /* offset guard, not used */
    idx_t m = 2 * n;          /* start value */
    seq_t val = 0;            /* result accumulator */

    while (m != 1)
    {
        if ((m & 1) == 0)     /* even: halving step */
        {
            m >>= 1;
            val = (val << 1) | 1;   /* append bit '1' */
        }
        else                 /* odd: 3x+1 step */
        {
            m = 3 * m + 1;
            val <<= 1;                 /* append bit '0' */
        }
    }
    return val;
}
