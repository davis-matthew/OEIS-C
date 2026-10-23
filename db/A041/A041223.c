/*
 * A041223  Denominators of continued fraction convergents to sqrt(123).
 * https://oeis.org/A041223
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 244*a(n-2) - a(n-4) Initial values: a(0)=1, a(1)=11,
 *            a(2)=243, a(3)=2684
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return the first four terms directly. */
    if (n == 0U) return (seq_t)1;
    if (n == 1U) return (seq_t)11;
    if (n == 2U) return (seq_t)243;
    if (n == 3U) return (seq_t)2684;

    /* Keep the last four values: a(i-4), a(i-3), a(i-2), a(i-1) */
    seq_t v[4] = {(seq_t)1, (seq_t)11, (seq_t)243, (seq_t)2684};

    /* Iterate from 4 up to n. */
    idx_t i;
    for (i = 4U; i <= n; ++i) {
        /* Compute a(i) = 244*a(i-2) - a(i-4) */
        seq_t val = (seq_t)244 * v[(i - 2U) % 4U] - v[(i - 4U) % 4U];
        v[i % 4U] = val;
    }
    return v[n % 4U];
}
