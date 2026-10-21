/*
 * A089146  a(n) = gcd(n^2 - 4, n^2 + 4).
 * https://oeis.org/A089146
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, mult
 * Formula:   a(n) = gcd(n^2 − 4 , n^2 + 4) (periodic: 4,1,8,1,4,1,…)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Only the remainder of n modulo 4 matters – the result is
       4 if n ≡ 0 (mod 4), 8 if n ≡ 2 (mod 4), otherwise 1. */
    switch (n & 3u) {
        case 0u: return (seq_t)4;
        case 1u: return (seq_t)1;
        case 2u: return (seq_t)8;
        default: return (seq_t)1;
    }
}
