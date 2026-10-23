/*
 * A101825  G.f.: x*(1+x)^2/(1-x^3).
 * https://oeis.org/A101825
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0; a(n) = (n%3==2) ? 2 : 1 for n>0
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;
    }
    /* Since idx_t is unsigned, modulus is cheap */
    idx_t r = n % 3;
    return (r == 2) ? (seq_t)2 : (seq_t)1;
}
