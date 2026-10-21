/*
 * A205129  Least k such that n divides a difference between distinct
 *          hexagonal numbers, ordered as in A205128.
 * https://oeis.org/A205129
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Enumerate the differences H_j - H_i (i < j) where H_k = k*(2k-1)
 *            is the k-th hexagonal number, in order of increasing j and then
 *            increasing i. a(n) is the index of the first difference
 *            divisible by n
 * Range:     u8 n <= 31, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   hexagonal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/hexagonal.h"

seq_t a(idx_t n)
{
    /* Edge case: for safety if n == 0 (not expected in test set) */
    if (n == 0) return 0;
    seq_t nm = (seq_t)n;
    idx_t counter = 0;            /* current rank (1‑based) */

    for (idx_t j = 2; ; ++j) {
        seq_t Hj = hexagonal(j);
        for (idx_t i = 1; i < j; ++i) {
            seq_t Hi = hexagonal(i);
            seq_t diff = Hj - Hi;   /* strictly positive */
            ++counter;
            if (diff % nm == 0)          /* n divides diff */
                return (seq_t)counter;   /* first such index */
        }
    }
}
