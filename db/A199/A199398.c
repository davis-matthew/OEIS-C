/*
 * A199398  XOR of the first n odd numbers.
 * https://oeis.org/A199398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = XOR_{k=1..n} (2k-1)
 * Range:     u8 n <= 128, u16 n <= 7900, u32 n <= 7900, u64 n <= 7900,
 *            u128 n <= 7900, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t xor_upto(idx_t m)
{
    switch (m & 3u)
    {
        case 0: return (seq_t)m;          /* m % 4 == 0 -> m */
        case 1: return (seq_t)1;          /* m % 4 == 1 -> 1 */
        case 2: return (seq_t)(m + 1);    /* m % 4 == 2 -> m+1 */
        default: return (seq_t)0;         /* m % 4 == 3 -> 0 */
    }
}

seq_t a(idx_t n)
{
    seq_t xor_2n = xor_upto(2 * n);
    seq_t xor_n  = xor_upto(n);
    return xor_2n ^ (xor_n << 1);
}
