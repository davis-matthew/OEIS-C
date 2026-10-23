/*
 * A182652  Algorithm: x->[xr] if x even, x->[x/r] is x odd, where r=sqrt(3)
 *          and [ ]=floor; a(n) is the number of steps from n to 0.
 * https://oeis.org/A182652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of steps needed to reach 0 by repeatedly applying
 *            the rule: if x is even, x := ⌊x·√3⌋; if x is odd, x := ⌊x/√3⌋.
 *            The function a(n) returns this step count for any n ≥ 1
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;          /* current value */
    seq_t steps = 0;             /* step counter */

    while (x != 0)
    {
        ++steps;

        /* compute ⌊x·√3⌋ by sqrt(3·x²) */
        seq_t temp = floor_sqrt((seq_t)3 * x * x);

        if ((x & (seq_t)1) == 0)   /* even */
            x = temp;
        else                       /* odd */
            x = temp / (seq_t)3;
    }

    return steps;
}
