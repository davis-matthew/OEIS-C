/*
 * A187050  Number of 6-step one or two space at a time bishop's tours on an n
 *          X n board summed over all starting positions.
 * https://oeis.org/A187050
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 16752*n^2 - 163720*n + 397436 for n > 9 Initial terms for
 *            1 ≤ n ≤ 9: a(1)=0, a(2)=0, a(3)=0, a(4)=584, a(5)=6952,
 *            a(6)=29500, a(7)=79088, a(8)=162320, a(9)=281320
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For small n use the pre‑computed values */
    if (n <= 9)
    {
        static const seq_t init[9] =
        {
            0,          /* a(1) */
            0,          /* a(2) */
            0,          /* a(3) */
            584,        /* a(4) */
            6952,       /* a(5) */
            29500,      /* a(6) */
            79088,      /* a(7) */
            162320,     /* a(8) */
            281320      /* a(9) */
        };
        return init[n - 1];
    }

    /* For n > 9 the closed form holds */
    seq_t nn = (seq_t)n;
    return (seq_t)16752 * nn * nn - (seq_t)163720 * nn + (seq_t)397436;
}
