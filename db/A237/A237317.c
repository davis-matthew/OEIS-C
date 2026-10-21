/*
 * A237317  Number of (n+1) X (1+1) 0..2 arrays with the maximum plus the
 *          upper median plus the minimum minus the lower median of every 2 X
 *          2 subblock differing from its horizontal and vertical neighbors by
 *          exactly one.
 * https://oeis.org/A237317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 13, u64 n <= 28, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A237317: Number of (n+1)x(2) 0..2 arrays with the imposed 2x2 rule
   Empirical recurrence: a(n) = 356 * a(n-4)  for n > 5
   Initial values:
     a(1) = 81   a(2) = 348   a(3) = 1508   a(4) = 6608   a(5) = 28772
*/
#include <stddef.h>

seq_t a(idx_t n)
{
    /* base values (index 1..5) */
    static const seq_t init[6] = {0, 81, 348, 1508, 6608, 28772};

    if (n <= 5)
        return init[n];

    /* determine the base index (2,3,4 or 5) and how many steps of 356^k */
    idx_t r = n % 4;
    idx_t base_index;
    if (r == 0)
        base_index = 4;
    else if (r == 1)
        base_index = 5;
    else
        base_index = r;   /* r == 2 or 3 */

    /* number of times we need to multiply by 356 */
    idx_t k = (n - base_index) / 4;

    /* compute 356^k */
    seq_t p = 1;
    for (idx_t i = 0; i < k; ++i)
        p *= 356;

    return p * init[base_index];
}
