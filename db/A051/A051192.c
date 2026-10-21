/*
 * A051192  T(n,7), array T as in A050186; a count of aperiodic binary words.
 * https://oeis.org/A051192
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * floor( binomial(n-1, 6) / 7 )
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 7) return 0;  /* offset starts at 7 */

    /* Compute binomial(n-1, 6) = (n-1)(n-2)...(n-6) / 720 */
    seq_t prod = 1;
    for (idx_t i = 1; i <= 6; ++i) {
        prod *= (seq_t)(n - i);
    }
    seq_t binom = prod / (seq_t)720;

    /* Divide by 7, floor automatically by integer division */
    seq_t part = binom / (seq_t)7;

    /* Multiply by n */
    return part * (seq_t)n;
}
