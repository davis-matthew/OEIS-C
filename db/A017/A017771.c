/*
 * A017771  Binomial coefficients C(55,n).
 * https://oeis.org/A017771
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = binom(55,n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return 0 for n out of the range [0,55] */
    if (n > 55u)
        return 0;

    /* Use symmetry: C(55,n) = C(55,55-n) to keep the loop short */
    idx_t k = n;
    if (k > 55 - k)
        k = 55 - k;

    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i) {
        result = result * (seq_t)(55 - k + i) / (seq_t)i;
    }
    return result;
}
