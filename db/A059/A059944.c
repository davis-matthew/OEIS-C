/*
 * A059944  Denominators of Maclaurin series coefficients for 2*cos(x/sqrt(3)
 *          + arctan(-sqrt(3))) = cos(x/sqrt(3)) + sqrt(3)*sin(x/sqrt(3)).
 * https://oeis.org/A059944
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = 3^floor(n/2) * n! (OEIS A059944)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* result = 1 for n = 0 (empty product) */
    seq_t res = 1;

    /* multiply by i for each i = 1..n, and by an extra factor 3 for even i */
    for (idx_t i = 1; i <= n; ++i) {
        res *= (seq_t)i;           /* multiply by i */
        if ((i & 1) == 0) {        /* i even → multiply by 3 */
            res *= (seq_t)3;
        }
    }
    return res;
}
