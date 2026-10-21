/*
 * A227316  a(n) = n(n+1) if n == 0 or 1 (mod 4), otherwise a(n) = n(n+1)/2.
 * https://oeis.org/A227316
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1) if n%4==0 or 1, otherwise a(n) = n*(n+1)/2
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;          /* cast index to sequence type */
    seq_t prod = N * (N + 1);    /* n*(n+1) */
    /* n%4 == 0 or 1 ? use prod, else divide by 2 */
    if ((n & 3ull) <= 1ull)
        return prod;            /* n mod 4 == 0 or 1 */
    return prod / 2;            /* n mod 4 == 2 or 3 */
}
