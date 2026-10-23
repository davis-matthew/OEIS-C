/*
 * A156661  Denominator of Euler(n, 1/19).
 * https://oeis.org/A156661
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = 19^n * 2^v₂(n+1), where v₂(x) is the exponent of 2 in x
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 14, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute v₂(n+1) */
    idx_t m = n + 1;
    int v = 0;
    while ((m & 1) == 0) {
        m >>= 1;
        v++;
    }

    /* compute 19^n */
    seq_t result = (seq_t)1;
    for (idx_t i = 0; i < n; ++i)
        result *= (seq_t)19;

    /* multiply by 2^v */
    result <<= v;
    return result;
}
