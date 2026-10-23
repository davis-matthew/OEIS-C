/*
 * A185826  Sum of the next n natural numbers raised to the n-th power.
 * https://oeis.org/A185826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((n + n^3)/2)^n (n ≥ 1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 12,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1; behavior for n==0 is undefined, but return 0 */
        return (seq_t)0;

    seq_t nn   = (seq_t)n;
    seq_t base = (nn + nn * nn * nn) / 2;   /* (n + n³)/2 in seq_t */
    return big_pow(base, nn);              /* raise base to the n‑th power */
}
