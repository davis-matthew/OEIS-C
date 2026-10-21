/*
 * A227140  a(n) = n/gcd(n,2^5), n >= 0.
 * https://oeis.org/A227140
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy, mult
 * Formula:   a(n) = n/gcd(n,32)
 * Range:     u8 n <= 256, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;
    }
    uint64_t t = 0;
    uint64_t temp = n;
    while ((temp & 1ULL) == 0 && t < 5) {
        t++;
        temp >>= 1;
    }
    return (seq_t)(n >> t);
}
