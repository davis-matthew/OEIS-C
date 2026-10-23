/*
 * A195035  Multiples of 15 and of 8 interleaved: a(2n-1) = 15n, a(2n) = 8n.
 * https://oeis.org/A195035
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2n-1) = 15n, a(2n) = 8n
 * Range:     u8 n <= 34, u16 n <= 8738, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if ((n & 1ULL) != 0ULL) {
        /* n is odd: n = 2k-1  => k = (n+1)/2  => a = 15k */
        seq_t kval = (seq_t)((n + 1) / 2);
        return 15 * kval;
    } else {
        /* n is even: n = 2k  => k = n/2  => a = 8k */
        seq_t kval = (seq_t)(n / 2);
        return 8 * kval;
    }
}
