/*
 * A010958  Binomial coefficient C(42,n).
 * https://oeis.org/A010958
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = binomial(42,n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n > 42)
        return 0;
    /* Use symmetry to reduce the number of steps. */
    idx_t k = n;
    if (k > 42 - k)
        k = 42 - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* Multiply by (42 - (k - i)) and then divide by i.  */
        res = res * ((seq_t)42 - (seq_t)(k - i));
        res = res / (seq_t)i;
    }
    return res;
}
