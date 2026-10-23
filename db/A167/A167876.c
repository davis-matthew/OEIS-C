/*
 * A167876  A000004 preceded by 1, 3, 4, 2.
 * https://oeis.org/A167876
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = coefficient of x^n in (1+x)*(1+2*x+2*x^2)
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* coefficients of the two factor polynomials */
    const seq_t c1[2] = { (seq_t)1, (seq_t)1 };    /* 1 + x */
    const seq_t c2[3] = { (seq_t)1, (seq_t)2, (seq_t)2 }; /* 1 + 2x + 2x^2 */

    seq_t res = 0;
    /* sum over i + j = n */
    for (idx_t i = 0; i <= 1; ++i) {
        idx_t j = n - i;
        if (j <= 2) {
            /* multiply the corresponding coefficients */
            res += c1[i] * c2[j];
        }
    }
    return res;
}
