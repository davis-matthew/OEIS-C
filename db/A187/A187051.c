/*
 * A187051  Number of 7-step one or two space at a time bishop's tours on an n
 *          X n board summed over all starting positions.
 * https://oeis.org/A187051
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + a(n-3) for n > 14. Initial values:
 *            a(1)=0, a(2)=0, a(3)=0, a(4)=400, a(5)=14024, a(6)=86608,
 *            a(7)=285048, a(8)=668176, a(9)=1272720, a(10)=2110944,
 *            a(11)=3180724, a(12)=4475176, a(13)=5989600, a(14)=7722744,
 *            a(15)=9674608
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return 0 for n <= 0 (outside the defined offset, harmless). */
    if (n == 0) return (seq_t)0;

    /* Known first 15 values. */
    static const seq_t init[15] = {
        0, 0, 0, 400,      /* a(1) ... a(4) */
        14024, 86608, 285048, 668176, /* a(5) ... a(8) */
        1272720, 2110944, 3180724, /* a(9) ... a(11) */
        4475176, 5989600, 7722744, /* a(12) ... a(14) */
        9674608                      /* a(15) */
    };

    /* For n <= 15 we can answer directly. */
    if (n <= 15) {
        return init[n - 1];
    }

    /* Use recurrence for n > 15.  Start with a(12), a(13), a(14). */
    seq_t a_m3 = init[12 - 1]; /* a(12) */
    seq_t a_m2 = init[13 - 1]; /* a(13) */
    seq_t a_m1 = init[14 - 1]; /* a(14) */
    seq_t val = 0;

    for (idx_t i = 15; i <= n; ++i) {
        /* a(i) = 3*a(i-1) - 3*a(i-2) + a(i-3) */
        val = 3 * a_m1 - 3 * a_m2 + a_m3;

        /* Shift the window forward. */
        a_m3 = a_m2;
        a_m2 = a_m1;
        a_m1 = val;
    }
    return val;
}
