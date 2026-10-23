/*
 * A115219  Expansion of 2*x^2*(1-x)/(1-3*x+2*x^2-2*x^3).
 * https://oeis.org/A115219
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=0, a(2)=2, a(3)=4 a(n+3) = 3*a(n+2) – 2*a(n+1) +
 *            2*a(n) (for n ≥ 0)
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 25, u64 n <= 49, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* hard‑coded initial values */
    static const seq_t init[4] = { 0, 0, 2, 4 };

    if (n < 4) return init[n];

    /* a_im1 = a(n‑1), a_im2 = a(n‑2), a_im3 = a(n‑3) */
    seq_t a_im1 = init[3];   /* a(3) */
    seq_t a_im2 = init[2];   /* a(2) */
    seq_t a_im3 = init[1];   /* a(1) */

    for (idx_t i = 4; i <= n; ++i) {
        /* a(i) = 3*a(i‑1) – 2*a(i‑2) + 2*a(i‑3) */
        seq_t term = 3 * a_im1;
        term -= 2 * a_im2;
        term += 2 * a_im3;

        /* shift window for next iteration */
        a_im3 = a_im2;
        a_im2 = a_im1;
        a_im1 = term;
    }

    return a_im1;
}
