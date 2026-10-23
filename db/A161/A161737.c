/*
 * A161737  Numerators of the column sums of the BG2 matrix.
 * https://oeis.org/A161737
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, frac, nonn
 * Formula:   a(n) = 2^((4n-5) + 4*v2((n-1)!) - v2(n-1) - 2*v2((2n-2)!)) where
 *            v2(m) is the exponent of 2 in m, v2(k!) = floor(k/2)+floor(k/4)+
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 34,
 *            big any n
 * Helpers:   pow2, v2_fact
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/v2_fact.h"

/* compute exponent of 2 in an unsigned integer x */
static seq_t v2_uint(idx_t x)
{
    seq_t c = 0;
    while ((x & 1) == 0) {
        ++c;
        x >>= 1;
    }
    return c;
}

/* seq_t a(idx_t n) : numerator of column sums of BG2 matrix */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;          /* sb(1) = 1 */

    /* base exponent from 2^(4n-5) */
    seq_t exp = (seq_t)(4 * n - 5);

    /* add 4 * v2((n-1)!)  */
    exp += 4 * v2_fact(n - 1);

    /* subtract v2(n-1)         */
    exp -= v2_uint(n - 1);

    /* subtract 2 * v2((2n-2)! ) */
    exp -= 2 * v2_fact(2 * n - 2);

    /* final numerator: 2^exp */
    return pow2((idx_t)exp);
}
