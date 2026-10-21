/*
 * A077858  Expansion of (1-x)^(-1)/(1-2*x+2*x^2-2*x^3).
 * https://oeis.org/A077858
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 23, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A077858 –  a(n) = 3·a(n‑1) – 4·a(n‑2) + 4·a(n‑3) – 2·a(n‑4)
    with a(0)=1, a(1)=3, a(2)=5, a(3)=7.   */
/*  The recurrence follows from the expansion of (1‑x)⁻¹/(1‑2x+2x²‑2x³).  */
#include <stdint.h>

/* Prefer a small O(n) loop.  For arbitrary‑precision seq_t the arithmetic
   operators are defined; for 64‑bit builds we use 128‑bit intermediates
   to avoid intermediate overflow.  */
seq_t a(idx_t n)
{
    /* handle small indices explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)7;

    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)3;
    seq_t a2 = (seq_t)5;
    seq_t a3 = (seq_t)7;
    seq_t ai;

    for (idx_t i = 4; i <= n; ++i) {
        /*  Use a 128‑bit intermediate to keep any 64‑bit implementation
            from overflow before the final conversion back to seq_t.  */
        unsigned __int128 temp =
            3 * (unsigned __int128)a3
          - 4 * (unsigned __int128)a2
          + 4 * (unsigned __int128)a1
          - 2 * (unsigned __int128)a0;

        ai = (seq_t)temp;    /* cast back to the required seq_t type */

        /* slide the window of the last four terms */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = ai;
    }
    return a3;
}
