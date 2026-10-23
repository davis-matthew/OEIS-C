/*
 * A147688  a(n) = ((6 + sqrt(8))^n + (6 - sqrt(8))^n)/2.
 * https://oeis.org/A147688
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*a(n-1) - 28*a(n-2), a(0)=1, a(1)=6
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;

    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)6;

    for (idx_t i = 2; i <= n; ++i) {
        seq_t ai = (seq_t)12 * a1 - (seq_t)28 * a0;
        a0 = a1;
        a1 = ai;
    }
    return a1;
}
