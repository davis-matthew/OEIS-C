/*
 * A010948  Binomial coefficient C(32,n).
 * https://oeis.org/A010948
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = C(32, n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 12, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the binomial coefficient (32 choose n).  For n > 32
   the result is defined to be 0. */
seq_t a(idx_t n)
{
    /* if n is outside the valid range, return 0 */
    if (n > 32u)
        return (seq_t)0;

    /* use symmetry: C(32, n) == C(32, 32-n) */
    idx_t k = n;
    if (k > 32u - k)
        k = 32u - k;

    if (k == 0u)
        return (seq_t)1;

    seq_t result = (seq_t)1;
    /* multiply and divide incrementally to avoid overflow of factorials */
    for (idx_t i = 1; i <= k; ++i) {
        result = result * (seq_t)(32u - k + i);
        result = result / (seq_t)i;
    }

    return result;
}
