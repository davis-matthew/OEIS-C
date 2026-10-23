/*
 * A047786  a(n) = (9*n^4 + 4*n^3 - n)/2.
 * https://oeis.org/A047786
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n^4 + 4*n^3 - n)/2
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 175, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    __uint128_t N = n;
    __uint128_t n2 = N * N;        /* n^2 */
    __uint128_t n3 = n2 * N;       /* n^3 */
    __uint128_t n4 = n3 * N;       /* n^4 */
    __uint128_t num = 9 * n4 + 4 * n3 - N;
    return (seq_t)(num / 2);
}
