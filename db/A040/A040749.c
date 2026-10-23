/*
 * A040749  Continued fraction for sqrt(777).
 * https://oeis.org/A040749
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   a(0)=27; for n≥1, a(n) = 6 when n≡2 (mod 4), * a(n) = 54 when
 *            n≡0 (mod 4) (i.e. divisible by 4), * and a(n) = 1 when n is odd
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)                 /* special offset entry */
        return (seq_t)27;

    /* check if n is odd */
    if ((n & 1ULL) == 0) {       /* even n */
        if ((n & 3ULL) == 2)     /* v₂(n) == 1 → n ≡ 2 (mod 4) */
            return (seq_t)6;
        else                     /* v₂(n) ≥ 2 → n ≡ 0 (mod 4) */
            return (seq_t)54;
    } else                       /* odd n */
        return (seq_t)1;
}
