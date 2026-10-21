/*
 * A067459  Sum of the remainders when n^2 is divided by squares less than n.
 * https://oeis.org/A067459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=1..n-1} (n^2 mod k^2)
 * Range:     u8 n <= 12, u16 n <= 78, u32 n <= 3215, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1) {
        return 0;
    }
    seq_t result = 0;
    seq_t n2 = (seq_t)n * (seq_t)n;   /* n^2 as seq_t */
    for (idx_t k = 1; k < n; ++k) {
        seq_t k2 = (seq_t)k * (seq_t)k;   /* k^2 as seq_t */
        result += n2 % k2;
    }
    return result;
}
