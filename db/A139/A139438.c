/*
 * A139438  Frieze pattern with 5 rows, read by diagonals.
 * https://oeis.org/A139438
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) is the n‑th diagonal entry of the unique frieze pattern
 *            with 5 rows, read by diagonals
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: the pattern is periodic with period 15: [1,1,2,3,1,1,3,5,2,1,1,2,1,1,1]. */
#include <stddef.h>
seq_t a(idx_t n)
{
    static const seq_t period[15] = {
        (seq_t)1, (seq_t)1, (seq_t)2, (seq_t)3, (seq_t)1,
        (seq_t)1, (seq_t)3, (seq_t)5, (seq_t)2, (seq_t)1,
        (seq_t)1, (seq_t)2, (seq_t)1, (seq_t)1, (seq_t)1
    };
    return period[(size_t)(n % 15)];
}
