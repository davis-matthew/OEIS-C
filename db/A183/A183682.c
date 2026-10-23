/*
 * A183682  Number of (n+1) X 3 binary arrays with every 2 X 2 subblock
 *          nonsingular.
 * https://oeis.org/A183682
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 4*a(n-2), a(1)=12, a(2)=32
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 21, u64 n <= 45, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)0;               /* offset starts at 1, value unused for n==0 */
    if (n == 1)
        return (seq_t)12;
    if (n == 2)
        return (seq_t)32;

    seq_t prev = (seq_t)12;             /* a(1) */
    seq_t curr = (seq_t)32;             /* a(2) */
    for (idx_t i = 3; i <= n; ++i) {
        seq_t next = curr + (seq_t)4 * prev;
        prev = curr;
        curr = next;
    }
    return curr;
}
