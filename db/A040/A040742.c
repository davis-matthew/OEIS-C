/*
 * A040742  Continued fraction for sqrt(770).
 * https://oeis.org/A040742
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   a(n) is multiplicative with a(2)=2, a(2^e)=54 for e≥2, a(p^e)=1
 *            for odd prime p; a(0)=27, a(1)=1
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)27;
    if (n == 1) return (seq_t)1;
    /* exponent of 2 dividing n */
    idx_t e = 0;
    while ((n & 1ULL) == 0) {
        n >>= 1;
        ++e;
    }
    if (e == 1)     return (seq_t)2;
    if (e >= 2)     return (seq_t)54;
    return (seq_t)1;
}
