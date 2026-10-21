/*
 * A074890  a(n) is the action of recursively applying 'Rule 30' elementary
 *          cellular automata on the binary representation of n if the cells
 *          may only expand into the significant bit, a(0) = 1.
 * https://oeis.org/A074890
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(0) = 1, a(n) = A387312(a(n-1))
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A387312 is the binary “Rule‑30” transformation described in OEIS A074890. */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t cur = (seq_t)1;                /* a(0) = 1 */
    for (idx_t i = (idx_t)1; i <= n; ++i)
    {
        /* Perform the Rule‑30 update on the binary representation of cur.  */
        /*  The right‑shift and left‑shift operations need a native integer
           shift amount; the second operand must not be of type seq_t.      */
        const unsigned long one = 1U;    /* safe shift amount for all built‑ins   */
        seq_t left  = cur << one;        /* cur * 2  (shift left, inserts 0 LSB) */
        seq_t right = cur >> one;        /* shift right, discards LSB              */
        cur = (left | cur) ^ right;      /* new bits = (cur >> 1) xor (cur | cur<<1) */
    }
    return cur;
}
