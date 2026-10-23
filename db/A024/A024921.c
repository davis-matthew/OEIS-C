/*
 * A024921  a(n) = Sum_{k=1..n} floor((n/k)*floor(n/k)).
 * https://oeis.org/A024921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{k=1..n} floor((n/k) * floor(n/k))
 * Range:     u8 n <= 13, u16 n <= 200, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    /* trivial guard for n == 0, though a(0) is not part of the OEIS offset */
    if (n == 0) return 0;

    for (idx_t k = 1; k <= n; ++k)
    {
        /* m = floor(n/k) */
        idx_t m = n / k;
        /* add floor( (n * m) / k ) */
        result += (seq_t)n * (seq_t)m / (seq_t)k;
    }
    return result;
}
