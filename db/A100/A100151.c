/*
 * A100151  Structured snub dodecahedral numbers.
 * https://oeis.org/A100151
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 109*C3(n) + 58*C2(n) + n where Ck(n) = n choose k. This
 *            identity follows from a(n) = (1/6)*n*(109*n^2 - 153*n + 50)
 * Range:     u8 n <= 2, u16 n <= 15, u32 n <= 618, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t k = (seq_t)n;
    /* C2 = n*(n-1)/2 */
    seq_t c2 = k * (k - 1) / 2;
    /* C3 = n*(n-1)*(n-2)/6 */
    seq_t c3 = k * (k - 1) * (k - 2) / 6;
    return 109 * c3 + 58 * c2 + k;
}
