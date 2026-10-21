/*
 * A221875  Numbers m such that 10*m^2 - 6 is a square.
 * https://oeis.org/A221875
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n-th smallest positive integer m such that 10*m²−6
 *            is a square. The sequence is obtained from the Pell equation
 *            x²−10y² = −6. Two fundamental solutions (x₀,y₀) = (2,1) and
 *            (22,7) generate all solutions via the unit (19,6), i.e.
 *            (xₖ₊₁,yₖ₊₁) = (19xₖ+60yₖ, 6xₖ+19yₖ). The y‑coordinates fr...
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* indices */
    idx_t cur = 1;

    /* first branch: (xA, yA) = (2, 1)  */
    seq_t xA = 2, yA = 1;

    /* second branch: (xB, yB) = (22, 7) */
    seq_t xB = 22, yB = 7;

    while (1) {
        seq_t next;

        /* choose the smaller of the current y values */
        if (yA <= yB) {
            next = yA;
            /* update branch A */
            seq_t newx = 19 * xA + 60 * yA;
            seq_t newy = 6  * xA + 19 * yA;
            xA = newx;
            yA = newy;
        } else {
            next = yB;
            /* update branch B */
            seq_t newx = 19 * xB + 60 * yB;
            seq_t newy = 6  * xB + 19 * yB;
            xB = newx;
            yB = newy;
        }

        if (cur == n) return next;
        cur++;
    }
}
