/*
 * A040534  Continued fraction for sqrt(558).
 * https://oeis.org/A040534
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = n-th term of the continued fraction expansion of √558.
   a(0) = 23, a(1..) follow the periodic block
   [1,1,1,1,1,4,1,1,1,1,1,46] which repeats every 12 terms. */
#include <stddef.h>  /* for size_t */

seq_t a(idx_t n)
{
    static const seq_t period[12] = {
        (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)1,
        (seq_t)4, (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)1,
        (seq_t)1, (seq_t)46
    };
    if (n == 0)
        return (seq_t)23;
    size_t k = (size_t)((n - 1) % 12);
    return period[k];
}
