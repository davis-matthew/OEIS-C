/*
 * A150500  Number of walks within N^3 (the first octant of Z^3) starting at
 *          (0,0,0) and consisting of n steps taken from {(-1, -1, -1), (-1,
 *          -1, 1), (0, 1, 0), (1, 1, -1), (1, 1, 1)}.
 * https://oeis.org/A150500
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk, easy, changed
 * Formula:   * a(n) = ((9*n+4)*a(n-1) + (22*n*n-27*n-15)*a(n-2) + *
 *            3*(n-2)*(8*n-31)*a(n-3) - 45*(n-2)*(n-3)*a(n-4)) * /(n+1)*(n+1)
 *            for n ≥ 4, * a(0)=1, a(1)=2, a(2)=7, a(3)=25
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Known initial values */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)7;
    if (n == 3) return (seq_t)25;

    /* keep last four values */
    seq_t a_im4 = (seq_t)1;          /* a(n-4) */
    seq_t a_im3 = (seq_t)2;          /* a(n-3) */
    seq_t a_im2 = (seq_t)7;          /* a(n-2) */
    seq_t a_im1 = (seq_t)25;         /* a(n-1) */
    seq_t a_i    = 0;                /* a(n) */

    for (idx_t i = 4; i <= n; ++i)
    {
        idx_t term1_coeff = 9*i + 4;
        idx_t term2_coeff = 22*i*i - 27*i - 15;
        idx_t term3_coeff = 3*(i-2)*(8*i - 31);
        idx_t term4_coeff = 45*(i-2)*(i-3);
        /* denominator (i+1)^2 fits easily in 64 bits */
        idx_t base = i + 1;
        idx_t denom_idx = base * base;

        if (sizeof(seq_t) <= 8) /* 64‑bit or smaller */
        {
            unsigned __int128 num = 0;
            unsigned __int128 denom = (unsigned __int128)denom_idx;

            num = (unsigned __int128)term1_coeff * (unsigned __int128)a_im1
                + (unsigned __int128)term2_coeff * (unsigned __int128)a_im2
                + (unsigned __int128)term3_coeff * (unsigned __int128)a_im3
                - (unsigned __int128)term4_coeff * (unsigned __int128)a_im4;

            a_i = (seq_t)(num / denom);
        }
        else /* arbitrary‑precision type */
        {
            seq_t num = 0;
            seq_t denom = (seq_t)denom_idx;

            num = (seq_t)term1_coeff * a_im1
                + (seq_t)term2_coeff * a_im2
                + (seq_t)term3_coeff * a_im3
                - (seq_t)term4_coeff * a_im4;

            a_i = num / denom;
        }

        /* shift for next iteration */
        a_im4 = a_im3;
        a_im3 = a_im2;
        a_im2 = a_im1;
        a_im1 = a_i;
    }

    return a_i;
}
