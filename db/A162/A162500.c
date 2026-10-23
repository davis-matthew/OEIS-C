/*
 * A162500  Expansion of the polynomial (1-x^3) * (1-x^6) * (1-x^9) / (1-x)^3.
 * https://oeis.org/A162500
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = #{(i,j,k) | 0 ≤ i ≤ 2, 0 ≤ j ≤ 5, 0 ≤ k ≤ 8, i + j + k =
 *            n}
 * Range:     u8 n <= 15, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n > 15 the coefficient is zero (finite polynomial). */
    if (n > (idx_t)15)
        return (seq_t)0;

    seq_t count = 0;
    for (idx_t i = 0; i <= (idx_t)2 && i <= n; ++i) {
        idx_t max_j = (idx_t)5;
        if (n < i + max_j) max_j = n - i;
        for (idx_t j = 0; j <= max_j; ++j) {
            idx_t k = n - i - j;
            if (k <= (idx_t)8)          /* automatically k >= 0 because of loop bounds */
                ++count;
        }
    }
    return count;
}
