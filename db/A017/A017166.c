/*
 * A017166  a(n) = (9*n)^6.
 * https://oeis.org/A017166
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n)^6
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 4, u64 n <= 180, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nine = (seq_t)9;
    seq_t t = nine * (seq_t)n;          // t = 9*n
    seq_t r = t * t;                    // r = t^2
    r *= t;                             // r = t^3
    r *= t;                             // r = t^4
    r *= t;                             // r = t^5
    r *= t;                             // r = t^6
    return r;
}
