/*
 * A131572  a(0) = 0 and a(1) = 1, continued such that absolute values of 2nd
 *          differences equal the original sequence.
 * https://oeis.org/A131572
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0, a(n) = 2^⌊n/2⌋ for n ≥ 1 (equivalently a(n)=2*a(n-2),
 *            n>2)
 * Range:     u8 n <= 15, u16 n <= 31, u32 n <= 63, u64 n <= 127,
 *            u128 n <= 255, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow2(idx_t exp) {
    seq_t r = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i)
        r *= (seq_t)2;
    return r;
}

seq_t a(idx_t n) {
    if (n == (idx_t)0)
        return (seq_t)0;
    return pow2(n / (idx_t)2);
}
