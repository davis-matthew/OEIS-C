/*
 * A239359  Number of n X 6 0..2 arrays with no element equal to one plus the
 *          sum of elements to its left or one plus the sum of elements above
 *          it or one plus the sum of the elements diagonally to its
 *          northwest, modulo 3.
 * https://oeis.org/A239359
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 6*a(n-2) + 4*a(n-3) - a(n-4) for n>17 a(1)=7,
 *            a(2)=19, a(3)=39, a(4)=77, a(5)=156, a(6)=266, a(7)=409,
 *            a(8)=599, a(9)=852, a(10)=1191, a(11)=1635, a(12)=2213,
 *            a(13)=2944, a(14)=3837, a(15)=4910, a(16)=6178, a(17)=7657
 * Range:     u8 n <= 5, u16 n <= 31, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* The first seventeen terms are stored in 64‑bit constants so
       they do not overflow any seq_t type.  They are cast to seq_t
       only when used. */
    static const uint64_t init[18] = {
        0,          /* unused index 0 */
        7, 19, 39, 77, 156, 266, 409, 599, 852, 1191,
        1635, 2213, 2944, 3837, 4910, 6178, 7657
    };

    if (n <= 17)
        return (seq_t)init[n];

    /* Initialise the last four known terms (a(n‑4)…a(n‑1)) */
    seq_t a0 = (seq_t)init[14];   /* a(n-4)   */
    seq_t a1 = (seq_t)init[15];   /* a(n-3)   */
    seq_t a2 = (seq_t)init[16];   /* a(n-2)   */
    seq_t a3 = (seq_t)init[17];   /* a(n-1)   */

    for (idx_t i = 18; i <= n; ++i) {
        /* a(n) = 4*a(n-1) - 6*a(n-2) + 4*a(n-3) - a(n-4) */
        seq_t next = a3 * 4 + a1 * 4;  /* 4*a(n-1)+4*a(n-3)   */
        next -= a2 * 6;                /* -6*a(n-2)           */
        next -= a0;                    /* -a(n-4)             */

        /* slide the window */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }

    return a3;
}
