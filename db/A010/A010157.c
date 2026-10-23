/*
 * A010157  Continued fraction for sqrt(79).
 * https://oeis.org/A010157
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   a(0)=8; for n>0, a(n)=1 if n is odd, 7 if n≡2 (mod 4), 16 if n≡0
 *            (mod 4)
 * Range:     u8 n <= 9486, u16 n <= 9486, u32 n <= 9486, u64 n <= 9486,
 *            u128 n <= 9486, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)8;
    /* Count the exponent of 2 in n */
    idx_t exp = 0;
    while ((n & 1ULL) == 0ULL) {
        ++exp;
        n >>= 1;
    }
    if (exp == 0) return (seq_t)1;   /* odd n */
    if (exp == 1) return (seq_t)7;   /* n = 2 * odd */
    return (seq_t)16;                /* n has 2^e, e>=2 */
}
