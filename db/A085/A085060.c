/*
 * A085060  Integer reached in A085058.
 * https://oeis.org/A085060
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 3 a(2*n) = 9*n + 3 a(2*n+1) = 3*a(n) + 3
 * Range:     u8 n <= 14, u16 n <= 510, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)3;

    if ((n & 1) == 0) {
        /* even index: a(2*n) = 9*n + 3 */
        seq_t half = (seq_t)(n / 2);
        return half * (seq_t)9 + (seq_t)3;
    } else {
        /* odd index:  a(2*n+1) = 3*a(n) + 3 */
        return (seq_t)3 * a((n - 1) / 2) + (seq_t)3;
    }
}
