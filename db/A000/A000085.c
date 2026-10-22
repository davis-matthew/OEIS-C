/*
 * A000085  Number of self-inverse permutations on n letters, also known as
 *          involutions; number of standard Young tableaux with n cells.
 * https://oeis.org/A000085
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, core, easy, nice
 * Formula:   a(n) = a(n-1) + (n-1)*a(n-2), a(0) = 1, a(1) = 1
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 18, u64 n <= 31, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t prev = 1; /* a(0) */
    seq_t cur = 1;  /* a(1) */
    idx_t i;
    if (n < 2)
        return cur;
    for (i = 2; i <= n; i++) {
        seq_t next = cur + (seq_t)(i - 1) * prev;
        prev = cur;
        cur = next;
    }
    return cur;
}
