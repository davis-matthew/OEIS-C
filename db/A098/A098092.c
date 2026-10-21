/*
 * A098092  Unique solution to the [10,3] postage stamp problem (showing that
 *          V(10,3) = 155).
 * https://oeis.org/A098092
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n) = denomination of the n‑th stamp in the unique * optimal
 *            set for the [10,3] postage stamp problem * (a(0)=1, a(1)=2,
 *            a(2)=6, a(3)=8, a(4)=19, a(5)=28, * a(6)=40, a(7)=43, a(8)=91,
 *            a(9)=103). * For n ≥ 10 the sequence is undefined, and the
 *            function returns 0
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* static table of the 10 optimal denominations */
    static const seq_t stamps[10] = {1, 2, 6, 8, 19, 28, 40, 43, 91, 103};
    if (n < 10)
        return stamps[(size_t)n];
    return 0;   /* undefined for n ≥ 10 */
}
