/*
 * A092412  Fixed point of the morphism 0->11, 1->12, 2->13, 3->10, starting
 *          from a(1) = 1.
 * https://oeis.org/A092412
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, mult, nonn
 * Formula:   a(n) = (1 + v₂(n)) mod 4 where v₂(n) is the exponent of 2 in the
 *            prime factorisation of n (i.e. the number of trailing zero bits
 *            in the binary representation)
 * Range:     u8 n <= 7536, u16 n <= 7536, u32 n <= 7536, u64 n <= 7536,
 *            u128 n <= 7536, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Count trailing zeros (the exponent of 2).  */
    unsigned e = 0;
    idx_t t = n;
    /* n ≥ 1, so the loop will terminate.  */
    while ((t & 1) == 0) {
        ++e;
        t >>= 1;
    }
    /* a(n) = (1 + e) mod 4;  use bitwise AND for modulo 4.  */
    return ((seq_t)1 + (seq_t)e) & 3;
}
