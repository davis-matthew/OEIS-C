/*
 * A228386  Number of n X 4 binary arrays with top left value 1 and no two
 *          ones adjacent horizontally or vertically.
 * https://oeis.org/A228386
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + 9*a(n-2) - 5*a(n-3) - 4*a(n-4) + a(n-5);
 *            Initial values: a(1)=3, a(2)=12, a(3)=72, a(4)=382, a(5)=2104
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 26, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Short‑circuit for the first five terms */
    static const seq_t init[5] = {3, 12, 72, 382, 2104};

    if (n <= 5)
        return init[n - 1];

    /* We use a sliding five‑element buffer to apply the recurrence. */
    seq_t p0 = init[0];   /* a(n-5) */
    seq_t p1 = init[1];   /* a(n-4) */
    seq_t p2 = init[2];   /* a(n-3) */
    seq_t p3 = init[3];   /* a(n-2) */
    seq_t p4 = init[4];   /* a(n-1) */

    for (idx_t k = 6; k <= n; ++k) {
        seq_t next = 4 * p4 + 9 * p3 - 5 * p2 - 4 * p1 + p0;
        p0 = p1;
        p1 = p2;
        p2 = p3;
        p3 = p4;
        p4 = next;
    }
    return p4;
}
