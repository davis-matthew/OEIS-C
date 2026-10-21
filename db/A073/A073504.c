/*
 * A073504  A possible basis for finite fractal sequences: let u(1) = 1, u(2)
 *          = n, u(k) = floor(u(k-1)/2) + floor(u(k-2)/2); then a(n) =
 *          lim_{k->infinity} u(k).
 * https://oeis.org/A073504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A073504  
 *  u(1) = 1, u(2) = n;  
 *  u(k) = ⌊u(k-1)/2⌋ + ⌊u(k-2)/2⌋   for k ≥ 3  
 *  a(n) = lim_{k→∞} u(k)
 *
 *  Initial terms (for reference):
 *     a(1)=0, a(2)=0, a(3)=0, a(4)=2, a(5)=2, a(6)=2, a(7)=2,
 *     a(8)=4, a(9)=4, a(10)=4, a(11)=4, a(12)=6, a(13)=6, a(14)=8,
 *     a(15)=8, a(16)=10, a(17)=10, a(18)=10, a(19)=10, a(20)=12,
 *     a(21)=12, a(22)=12, a(23)=12, a(24)=14, …
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /*  Sequence starts with u(1)=1 and u(2)=n. */
    seq_t prev2 = 1;          /* u(k-2)   */
    seq_t prev  = (seq_t)n;   /* u(k-1)   */
    seq_t cur;

    for (;;)
    {
        /* compute u(k) = floor(prev/2)+floor(prev2/2) */
        cur = (prev >> 1) + (prev2 >> 1);

        /* stop when u(k) == u(k-1) == u(k-2) – the limit is reached */
        if (cur == prev && cur == prev2)
            return cur;

        prev2 = prev;
        prev  = cur;
    }
}
