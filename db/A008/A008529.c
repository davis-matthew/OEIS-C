/*
 * A008529  Coordination sequence for 4-dimensional face-centered cubic
 *          orthogonal lattice.
 * https://oeis.org/A008529
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            binom(n+3,3)+10*binom(n+2,3)+18*binom(n+1,3)+10*binom(n,3)+binom(n-1,3)
 * Range:     u8 n <= 3, u16 n <= 21, u32 n <= 863, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Binomial coefficient binom(m,3) is 0 for m<3.  */
seq_t a(idx_t n)
{
    static const seq_t coeff[5] = { (seq_t)1, (seq_t)10, (seq_t)18,
                                   (seq_t)10, (seq_t)1 };

    seq_t res = (seq_t)0;

    for (idx_t k = 0; k < 5; ++k)
    {
        /* Skip terms where n+3-k would be negative. */
        if (n + 3 < k) continue;

        idx_t m = n + 3 - k;         /* m = n + 3 - k  */

        /* compute binom(m,3)  */
        seq_t bin;
        if (m < 3)
            bin = (seq_t)0;
        else
        {
            seq_t sn = (seq_t)m;
            seq_t a_ = sn;
            seq_t b_ = sn - (seq_t)1;
            seq_t c_ = sn - (seq_t)2;

            /* divide by 2 first */
            if ((a_ % (seq_t)2) == 0)
                a_ /= (seq_t)2;
            else
                b_ /= (seq_t)2;

            /* divide by 3 next */
            if ((a_ % (seq_t)3) == 0)
                a_ /= (seq_t)3;
            else if ((b_ % (seq_t)3) == 0)
                b_ /= (seq_t)3;
            else
                c_ /= (seq_t)3;

            bin = a_ * b_ * c_;
        }

        res += coeff[k] * bin;
    }

    return res;
}
