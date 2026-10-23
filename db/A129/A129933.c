/*
 * A129933  a(n) = (n*(n+1)/2+1)!/n!.
 * https://oeis.org/A129933
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((n*(n+1))/2+1)! / n!
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* result starts at 1 */
    seq_t res = (seq_t)1;

    /* triangular number T = n*(n+1)/2, computed in seq_t */
    seq_t tri = (seq_t)n * ((seq_t)n + (seq_t)1) / (seq_t)2;

    /* upper bound for the product: T + 1, cast back to idx_t for the loop */
    idx_t end = (idx_t)(tri + (seq_t)1);

    /* multiply all integers from n+1 up to T+1 inclusive */
    for (idx_t k = n + (idx_t)1; k <= end; ++k)
    {
        res *= (seq_t)k;
    }

    return res;
}
