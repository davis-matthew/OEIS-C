/*
 * A040683  Continued fraction for sqrt(710).
 * https://oeis.org/A040683
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   a(0) = 26. For n ≥ 1, a(n) depends only on the exponent of 2 in
 *            n: let e = v₂(n) (maximal e with 2^e | n). a(n) = 1 if e = 0 or
 *            e = 1 a(n) = 4 if e = 2 a(n) = 52 if e ≥ 3 (This follows from
 *            the multiplicative definition a(2)=1, a(4)=4, a(2^e)=52 for e≥3,
 *            a(p^e)=1 for odd primes p.)
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)26;

    if ((n & 1) != 0) return (seq_t)1;          /* odd n → exponent e = 0 */

    /* count the exponent of 2 */
    idx_t e = 0;
    while ((n & 1) == 0) {
        n >>= 1;
        ++e;
    }

    if (e == 1)       return (seq_t)1;
    else if (e == 2)  return (seq_t)4;
    else              return (seq_t)52;
}
