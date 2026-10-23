/*
 * A165754  a(n) = nimsum(n+(n+1)+(n+2)).
 * https://oeis.org/A165754
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + n + 2*(-1)^n (equivalently a(2k) = 2k + 3, a(2k+1) =
 *            2k)
 * Range:     u8 n <= 253, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t to avoid mixed-type intermediate */
    seq_t N = (seq_t)n;

    /* For odd n, (-1)^n = -1 → a = n - 1
       For even n, (-1)^n =  1 → a = n + 3 */
    return (n & 1) ? N - 1 : N + 3;
}
