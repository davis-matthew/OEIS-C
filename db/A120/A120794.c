/*
 * A120794  Numerators of partial sums of Catalan numbers scaled by powers of
 *          -1/16.
 * https://oeis.org/A120794
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = numerator of the reduced fraction r(n), where
 *            r(n)=Sum_{k=0..n} (-1)^k * Catalan(k) / 16^k
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t r_num = 0;          /* current numerator */
    idx_t  r_exp = 0;         /* current denominator exponent: r_num / 2^r_exp */

    seq_t cat = 1;            /* Catalan(0) */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t term = cat;            /* C(k) */
        idx_t term_exp = k * 4;      /* 16^k = 2^(4k) */

        /* Align term with r_num to a common denominator 2^max_exp */
        if (r_exp >= term_exp) {
            idx_t shift = r_exp - term_exp;
            seq_t term_shift = term << shift;
            if ((k & 1) == 0) {
                r_num += term_shift;
            } else {
                r_num -= term_shift;          /* r_num >= term_shift holds */
            }
        } else {
            idx_t shift = term_exp - r_exp;
            seq_t r_shift = r_num << shift;
            seq_t new_num;
            if ((k & 1) == 0) {
                new_num = r_shift + term;
            } else {
                new_num = r_shift - term;          /* r_shift >= term holds */
            }
            r_num = new_num;
            r_exp = term_exp;
        }

        /* Reduce the fraction by removing common powers of two */
        while ((r_num & 1) == 0 && r_exp > 0) {
            r_num >>= 1;
            --r_exp;
        }

        /* Prepare Catalan(k+1) for the next iteration */
        if (k < n) {
            /* cat = cat * (4*k + 2) / (k + 2) */
            seq_t num = cat * ((seq_t)(4 * k + 2));
            cat = num / ((seq_t)(k + 2));
        }
    }

    return r_num;
}
