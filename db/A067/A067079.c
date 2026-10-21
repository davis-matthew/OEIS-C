/*
 * A067079  Right digitorial function Rd(n) = abcd...*bcd...*cd...*d...*...,
 *          for n = abcd... in decimal notation.
 * https://oeis.org/A067079
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn, nice, look
 * Formula:   a(n) = Product_{k=1}^{len(n)} (n mod 10^k) (where len(n) is the
 *            number of decimal digits of n)
 * Range:     u8 n <= 28, u16 n <= 148, u32 n <= 4998, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Determine the number of decimal digits of n.
       For n == 0 we want len = 1. */
    idx_t temp = n;
    idx_t len  = 0;
    do {
        len++;
        temp /= 10;
    } while (temp);

    /* Iterate over suffixes: for k = 1 .. len,
       use 10^k as the modulus. */
    idx_t pow10 = 10;      /* 10^1 */
    seq_t product = 1;

    for (idx_t k = 1; k <= len; ++k) {
        idx_t suffix = n % pow10;       /* last k digits of n   */
        product *= (seq_t)suffix;       /* multiply into the product  */
        if (product == 0)              /* early exit if zero occurs   */
            return 0;
        pow10 *= 10;                    /* next power of 10           */
    }
    return product;
}
