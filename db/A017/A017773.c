/*
 * A017773  Binomial coefficients C(57,n).
 * https://oeis.org/A017773
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = binomial(57,n) (0 ≤ n ≤ 57), a(n) = 0 for n > 57
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n > 57) return (seq_t)0;
    idx_t k = n;
    if (k > 57 - k) k = 57 - k;            /* use symmetry */
    seq_t result = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        result = result * ((seq_t)(57 - k + i)) / (seq_t)i;
    }
    return result;
}
