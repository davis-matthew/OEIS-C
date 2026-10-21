/*
 * A071232  a(n) = (n^6 + n^3)/2.
 * https://oeis.org/A071232
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^6 + n^3)/2 = n^3*(n^3 + 1)/2
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 45, u64 n <= 1824,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute n^3 in a 64‑bit variable; this fits for all n that we need to evaluate
       accurately when the result fits in seq_t (seq_t will be at least 64 bits
       for the builds that use 64‑bit output). */
    seq_t m = (seq_t)n * (seq_t)n * (seq_t)n;        /* m = n^3 */

    /* To avoid intermediate overflow we divide one of the two factors
       (m or m+1) by 2 before multiplication.  Since one of them is even,
       the division is always integral.  The remaining two factors are
       <= m and <= m+1 respectively, and their product is exactly
       n^3*(n^3+1)/2, which fits in seq_t for all n whose a(n) does. */
    if (m & (seq_t)1) {               /* m is odd */
        return m * ((m + (seq_t)1) >> 1);
    } else {                           /* m is even */
        return (m >> 1) * (m + (seq_t)1);
    }
}
