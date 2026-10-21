/*
 * A077107  Least integer cube >= n^2.
 * https://oeis.org/A077107
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = least integer cube >= n^2
 * Range:     u8 n <= 14, u16 n <= 252, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* literally: compute s = n*n; find smallest k with k^3 >= s; return k^3 */

seq_t a(idx_t n)
{
    /* compute n^2 in seq_t */
    seq_t s = (seq_t)n * (seq_t)(n + 1) / 2;  /* wrong, use n*n */
    /* correct calculation: */
    s = (seq_t)n * (seq_t)n;

    /* Special case: n==0 */
    if (s == 0)
        return 0;

    seq_t k = 1;
    /* simple loop: for the n-range considered (≤10k) k is small (~n^(2/3)),
     * so this is efficient.  In a 64‑bit build k^3 will never overflow for
     * the tested range. */
    while (k * k * k < s)
        ++k;

    return k * k * k;
}
