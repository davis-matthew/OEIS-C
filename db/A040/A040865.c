/*
 * A040865  Continued fraction for sqrt(895).
 * https://oeis.org/A040865
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   a(0) = 29; For n > 0: • if n is odd → a(n) = 1; • if n is even →
 *            let e = v₂(n) (exponent of 2 in n); then a(n) = 10 if e == 1
 *            else 58. The function is multiplicative with a(2)=10, a(2^e)=58
 *            for e≥2, and a(p^e)=1 for odd primes p
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)29;
    if (n & 1)   return (seq_t)1;          /* odd n → 1 */

    /* count exponent of 2 */
    idx_t e = 0;
    idx_t temp = n;
    while ((temp & 1) == 0) {
        ++e;
        temp >>= 1;
    }

    return (e == 1) ? (seq_t)10 : (seq_t)58;
}
