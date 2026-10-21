/*
 * A207048  Number of (n+1) X 7 0..2 arrays with every 2 X 3 or 3 X 2 subblock
 *          having exactly one clockwise edge increases.
 * https://oeis.org/A207048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-3) for n > 14. Initial values (1‑based):
 *            a(1)=13386, a(2)=4722, a(3)=3408, a(4)=4836, a(5)=7584,
 *            a(6)=11712, a(7)=19200, a(8)=30528, a(9)=48096, a(10)=78744,
 *            a(11)=125184, a(12)=196320, a(13)=316128, a(14)=500832. The
 *            recurrence holds for every n ≥ 15
 * Range:     u8 none, u16 n <= 9, u32 n <= 33, u64 n <= 81, u128 n <= 177,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init[15] = {
        0,          /* unused, 0‑th index */
        13386,      /* a(1) */
        4722,       /* a(2) */
        3408,       /* a(3) */
        4836,       /* a(4) */
        7584,       /* a(5) */
        11712,      /* a(6) */
        19200,      /* a(7) */
        30528,      /* a(8) */
        48096,      /* a(9) */
        78744,      /* a(10) */
        125184,     /* a(11) */
        196320,     /* a(12) */
        316128,     /* a(13) */
        500832      /* a(14) */
    };

    /* For n ≤ 14 return the stored initial value. */
    if (n <= 14) {
        return init[n];
    }

    /* Determine how many times we must apply the factor 4 and the base
       index (12,13 or 14) that lies in the known initial region. */
    idx_t steps = (n - (idx_t)12) / 3;          /* number of multiplications by 4 */
    idx_t base  = n - (steps * (idx_t)3);       /* base index in [12,14] */

    seq_t val = init[base];

    /* Apply the factor 4^steps.  For large n the number of iterations
       is at most n/3, which is small enough for the required time limits. */
    while (steps--) {
        val *= (seq_t)4;
    }
    return val;
}
