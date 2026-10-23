/*
 * A185273  Period 6: repeat [1, 6, 5, 6, 1, 0].
 * https://oeis.org/A185273
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Period 6: repeat [1, 6, 5, 6, 1, 0] for n ≥ 1 (offset 1) */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* sequence cycle */
    static const seq_t cycle[6] = { (seq_t)1, (seq_t)6, (seq_t)5,
                                    (seq_t)6, (seq_t)1, (seq_t)0 };
    /* compute index as (n-1) mod 6 */
    return cycle[(unsigned)((n - 1) % 6)];
}
