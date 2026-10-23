/*
 * A021619  Decimal expansion of 1/615.
 * https://oeis.org/A021619
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = digit n of decimal expansion of 1/615. The first five
 *            digits (preperiod) are 0,0,1,6,2. For n ≥ 5 the repeating cycle
 *            is 6,0,1,6,2
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* digits for n = 0..4 */
    static const seq_t first[5] = { (seq_t)0, (seq_t)0, (seq_t)1, (seq_t)6, (seq_t)2 };
    /* repeating cycle for n ≥ 5 */
    static const seq_t cycle[5] = { (seq_t)6, (seq_t)0, (seq_t)1, (seq_t)6, (seq_t)2 };

    if (n < 5)
        return first[(size_t)n];
    return cycle[(size_t)((n - 5) % 5)];
}
