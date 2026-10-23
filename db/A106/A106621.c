/*
 * A106621  a(n) = numerator of n/(n+20).
 * https://oeis.org/A106621
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac, mult
 * Formula:   a(n) = n/gcd(n,20) (numerator of n/(n+20))
 * Range:     u8 n <= 256, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline uint64_t gcd64(uint64_t a, uint64_t b)
{
    while (b) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* Use 64‑bit gcd on the index; n is uint64_t */
    uint64_t g = gcd64((uint64_t)n, 20u);
    /* Integer division is exact because g divides n */
    return (seq_t)((uint64_t)n / g);
}
