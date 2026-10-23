/*
 * A017790  Binomial coefficients C(74,n).
 * https://oeis.org/A017790
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = C(74,n) for 0 ≤ n ≤ 74, otherwise a(n) = 0
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 24, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n > 74) return (seq_t)0;

    /* Use symmetry to reduce the number of iterations */
    idx_t k = n;
    if (k > 74 - k) k = 74 - k;

    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* numerator for the i‑th term */
        idx_t num = 74 - k + i;
        /* use 128‑bit intermediate to avoid overflow in 64‑bit builds */
        unsigned __int128 tmp = (unsigned __int128)res * num;
        res = (seq_t)(tmp / i);
    }
    return res;
}
