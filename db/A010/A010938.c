/*
 * A010938  Binomial coefficient C(22,n).
 * https://oeis.org/A010938
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = binomial(22,n) (0 ≤ n ≤ 22, otherwise 0)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 22, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return 0 for indices that exceed the binomial upper bound */
    if (n > 22)
        return (seq_t)0;

    /* Take advantage of symmetry: C(22,n) = C(22,22-n) */
    idx_t k = n;
    if (k > 22 - k)
        k = 22 - k;

    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* Multiply by (22 - i + 1) and divide by i; division is exact */
        result = result * ((seq_t)(22 - i + 1)) / (seq_t)i;
    }
    return result;
}
