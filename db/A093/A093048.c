/*
 * A093048  a(n) = n minus exponent of 2 in n, with a(0) = 0.
 * https://oeis.org/A093048
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - v2(n) where v2(n) is the exponent of 2 in n; a(0) = 0
 * Range:     u8 n <= 256, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    unsigned long long exp = 0;
    idx_t tmp = n;
    /* count trailing zeros: the power of 2 dividing n */
    while ((tmp & 1ULL) == 0) {
        ++exp;
        tmp >>= 1;
    }

    return (seq_t)n - (seq_t)exp;
}
