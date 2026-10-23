/*
 * A171623  a(n) = floor(n-th composite / n).
 * https://oeis.org/A171623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = floor(composite(n) / n) where composite(n) is the n-th
 *            composite integer. Observation: composite(n) < 2n for all n >=
 *            8, so floor(composite(n)/n) = 1 for n >= 8. The first seven
 *            composites are 4, 6, 8, 9, 10, 12, 14
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Precomputed list of the first seven composite numbers */
    static const seq_t comps[7] = {4, 6, 8, 9, 10, 12, 14};

    if (n < 8) {
        /* For n = 1..7 compute floor(composite / n) */
        return comps[n - 1] / (seq_t)n;
    } else {
        /* For n >= 8 the value is always 1 */
        return 1;
    }
}
