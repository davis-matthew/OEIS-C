/*
 * A016134  Expansion of g.f. 1/((1-2*x)*(1-10*x)).
 * https://oeis.org/A016134
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)*10 + 2^n, a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t value   = 1; /* a(0) */
    seq_t pow2    = 1; /* 2^0 */

    for (idx_t i = 1; i <= n; ++i) {
        pow2 *= 2;            /* pow2 = 2^i */
        value = value * 10 + pow2; /* a(i) = a(i-1)*10 + 2^i */
    }

    return value;
}
