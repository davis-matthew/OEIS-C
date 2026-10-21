/*
 * A078891  Concatenate first n triangular numbers in reverse order.
 * https://oeis.org/A078891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, less
 * Formula:   a(n) = concatenate(T_k) for k=n down to 1, with T_k = k*(k+1)/2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the trivial case (offset 1, but guard anyway) */
    if (n == 0)
        return 0;

    seq_t result = 0;
    idx_t k = n;

    while (k > 0) {
        /* Compute the k‑th triangular number */
        seq_t tri = (seq_t)k * ((seq_t)k + 1) / 2;

        /* Count its decimal digits */
        idx_t digits = 0;
        seq_t tmp = tri;
        while (tmp > 0) {
            tmp /= 10;
            digits++;
        }

        /* Compute 10^digits */
        seq_t pow10 = 1;
        for (idx_t i = 0; i < digits; ++i)
            pow10 *= 10;

        /* Append tri to the right of the current result */
        result = result * pow10 + tri;
        k--;                   /* move to the next smaller triangular number */
    }

    return result;
}
