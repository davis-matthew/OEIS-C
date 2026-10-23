/*
 * A040370  Continued fraction for sqrt(390).
 * https://oeis.org/A040370
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   a(n) = 19 for n = 0; * a(n) = 1 if n has no factor 2 (v₂(n) =
 *            0); * a(n) = 2 if n has exactly one factor 2 (v₂(n) = 1); * a(n)
 *            = 38 if n has at least two factors 2 (v₂(n) ≥ 2); * This matches
 *            the multiplicative definition: a(p^e)=1 for odd p, a(2)=2, *
 *            a(2^e)=38 for e≥2
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)19;

    /* Count trailing zeros in binary representation of n
       (i.e. exponent of 2). */
    idx_t m = n;
    unsigned int e = 0;
    while ((m & 1) == 0) {
        e++;
        m >>= 1;
    }

    if (e == 0)
        return (seq_t)1;
    else if (e == 1)
        return (seq_t)2;
    else
        return (seq_t)38;
}
