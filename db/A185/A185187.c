/*
 * A185187  Smallest number for which the greedy algorithm fails to find the
 *          sum of n-th powers with at most A002804 terms.
 * https://oeis.org/A185187
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   For n >= 3: a(n) = 3^n + (floor(3^n / 2^n) - 1) * 2^n + (2^n -
 *            1) Special case: a(2) = 23 (offset 2) For n < 2 the value is
 *            unused
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 80,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* Unused indices below the offset */
    if (n < (idx_t)2)
        return 0;

    /* Special case for n = 2 */
    if (n == (idx_t)2)
        return (seq_t)23;

    /* Compute 3^n and 2^n using the provided utilities */
    seq_t pow3_n = pow3(n);
    seq_t pow2_n = pow2(n);

    /* floor(3^n / 2^n) */
    seq_t q = pow3_n / pow2_n;

    /* Apply the formula */
    seq_t res = pow3_n + (q - (seq_t)1) * pow2_n + (pow2_n - (seq_t)1);
    return res;
}
