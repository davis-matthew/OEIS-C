/*
 * A118014  a(n) = Sum_{k=1..n} floor(n^2/k).
 * https://oeis.org/A118014
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} floor(n^2/k)
 * Range:     u8 n <= 9, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    const seq_t ns = (seq_t)n;
    seq_t N = ns * ns;            /* N = n^2  (seq_t) */
    seq_t sum = 0;

    for (idx_t k = 1; k <= n; ) {
        const seq_t ks = (seq_t)k;
        seq_t q = N / ks;          /* q = floor(N/k)   */
        seq_t r = N / q;           /* r = floor(N/q)   */
        if (r > ns) r = ns;        /* clamp r to n      */

        sum += q * (r - ks + 1);

        k = (idx_t)(r + 1);
    }

    return sum;
}
