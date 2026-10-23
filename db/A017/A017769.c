/*
 * A017769  Binomial coefficients C(53,n).
 * https://oeis.org/A017769
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = C(53,n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n > 53) return 0;

    idx_t k = n;
    if (k > 53 - k) k = 53 - k;

    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i) {
        result = result * ((seq_t)(53 - k + i)) / (seq_t)i;
    }

    return result;
}
