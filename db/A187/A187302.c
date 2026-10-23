/*
 * A187302  Number of 7-step one space leftwards or up, two space rightwards
 *          or down asymmetric rook's tours on an n X n board summed over all
 *          starting positions.
 * https://oeis.org/A187302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2776*n^2 - 19580*n + 30728 for n >= 12; sequence terms
 *            for n = 1..11 are hard‑coded
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init[12] = {
        0,   /* a(0) unused */
        0,   /* a(1) */
        0,   /* a(2) */
        108, /* a(3) */
        1368,/* a(4) */
        6264,/* a(5) */
        15596,/* a(6) */
        31172,/* a(7) */
        52256,/* a(8) */
        79634,/* a(9) */
        112568,/* a(10) */
        151266 /* a(11) */
    };

    if (n < 12) {
        return init[n];
    }

    seq_t nn = (seq_t)n;
    seq_t res = 2776 * nn * nn;   /* 2776 * n^2 */
    res -= 19580 * nn;            /* -19580 * n */
    res += 30728;                 /* +30728 */
    return res;
}
