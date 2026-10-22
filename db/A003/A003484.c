/*
 * A003484  Radon function, also called Hurwitz-Radon numbers.
 * https://oeis.org/A003484
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, core, nice, mult
 * Formula:   a(n) = 8*b + 2^c where n = 2^(4*b+c)*d, 0<=c<=3, d odd
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the 2-adic valuation of n (highest power of 2 dividing n) */
    idx_t v2 = 0;
    while ((n & 1) == 0) {
        v2++;
        n >>= 1;
    }
    
    /* Split v2 = 4*b + c with 0 <= c <= 3 */
    idx_t b = v2 / 4;
    idx_t c = v2 % 4;
    
    /* Return 8*b + 2^c */
    seq_t power_of_2[] = {1, 2, 4, 8};
    return (seq_t)(8 * b + power_of_2[c]);
}
