/*
 * A134012  Period 5: repeat [1, 6, 11, 6, 1].
 * https://oeis.org/A134012
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A134012: a(n) = periodic sequence [1, 6, 11, 6, 1] with period 5 (offset 0) */
#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t tbl[5] = { (seq_t)1, (seq_t)6, (seq_t)11, (seq_t)6, (seq_t)1 };
    return tbl[(size_t)(n % (idx_t)5)];
}
