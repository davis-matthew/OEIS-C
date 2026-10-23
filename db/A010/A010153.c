/*
 * A010153  Continued fraction for sqrt(75) (or 5*sqrt(3)).
 * https://oeis.org/A010153
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   a(0) = 8; * a(n) = 16 if n is divisible by 4 (i.e., 4 | n); *
 *            a(n) = 1 otherwise. * This follows from the multiplicative
 *            definition: * a(2) = 1, a(2^e) = 16 for e ≥ 2, a(p^e) = 1 for
 *            odd primes p. * Hence any n divisible by 4 receives value 16,
 *            all other positive n give 1
 * Range:     u8 n <= 9567, u16 n <= 9567, u32 n <= 9567, u64 n <= 9567,
 *            u128 n <= 9567, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)8;
    /* test divisibility by 4: n ≡ 0 (mod 4) iff the two low bits are zero */
    if ((n & 3u) == 0u)
        return (seq_t)16;
    return (seq_t)1;
}
