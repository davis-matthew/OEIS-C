/*
 * A010945  Binomial coefficient C(29,n).
 * https://oeis.org/A010945
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = binomial coefficient C(29,n)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return 0 for out-of-range indices, following the usual convention */
    if (n > 29) return (seq_t)0;

    /* Use symmetry: C(29,n) == C(29,29-n) to minimize multiplicative steps */
    idx_t k = n;
    if (k > 29u - k) k = 29u - k;

    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* Multiply then divide; the division is exact at each step */
        result = result * (29u - k + i) / i;
    }
    return result;
}
