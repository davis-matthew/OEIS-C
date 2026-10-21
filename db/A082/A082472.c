/*
 * A082472  a(1) = 1, a(n) = Sum_{k=1..n-1} a(k)*2^k.
 * https://oeis.org/A082472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(2) = 2, a(n+1) = (2^n + 1) * a(n) for n >= 2
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 1;
    if (n == 2) return 2;

    seq_t res = 2;          /* a(2) */
    seq_t pow2 = 4;         /* 2^2, ready for n = 3 */

    for (idx_t i = 3; i <= n; ++i) {
        res = (pow2 + 1) * res;   /* a(i) = (2^(i-1) + 1) * a(i-1) */
        pow2 <<= 1;               /* advance 2^(i) for next iteration */
    }
    return res;
}
