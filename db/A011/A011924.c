/*
 * A011924  Floor[n(n-1)(n-2)(n-3)/14].
 * https://oeis.org/A011924
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor[n*(n-1)*(n-2)*(n-3)/14]
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 411, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 4) {
        return (seq_t)0;
    }
    /* Compute (n)*(n-1)*(n-2)*(n-3) as seq_t to avoid overflow */
    seq_t prod = ((seq_t)n) * ((seq_t)(n - 1)) *
                 ((seq_t)(n - 2)) * ((seq_t)(n - 3));
    return prod / (seq_t)14;
}
