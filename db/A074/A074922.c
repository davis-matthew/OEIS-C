/*
 * A074922  Number of ways of arranging n chords on a circle (handshakes
 *          between 2n people across a table) with exactly 2 simple
 *          intersections.
 * https://oeis.org/A074922
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(2n, n-2)*(n-2)/2
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 3) return (seq_t)0;          /* a(0)=a(1)=a(2)=0 */

    idx_t N = 2 * n;                     /* upper argument of the binomial */
    idx_t k = n - 2;                     /* amount to choose */

    seq_t res = (seq_t)1;
    /* compute binomial(2n, n-2) multiplicatively */
    for (idx_t i = 1; i <= k; ++i) {
        /* multiply by (N - k + i) then divide by i – stays integral */
        res = res * (seq_t)(N - k + i) / (seq_t)i;
    }

    res = res * (seq_t)(n - 2);          /* multiply by (n-2) */
    res = res / (seq_t)2;                /* divide by 2 */
    return res;
}
