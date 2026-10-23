/*
 * A029150  Expansion of 1/((1-x^2)*(1-x^3)*(1-x^6)*(1-x^7)).
 * https://oeis.org/A029150
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n^3+27*n^2+306*n+1512)/1512 - (n+9)*(n mod 2)/24 -
 *            n*((n^2) mod 3)/18)
 * Range:     u8 none, u16 n <= 31, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t nseq = (seq_t)n;

    /* first term: (n^3 + 27*n^2 + 306*n + 1512)/1512
     *  multiplied by the common denominator 1512
     *  to keep everything as integers */
    seq_t num1 = nseq*nseq*nseq
               + (seq_t)27*nseq*nseq
               + (seq_t)306*nseq
               + 1512;

    /* second term: ((n+9)*(n mod 2))/24
     *  multiplied by 1512/24 = 63 */
    seq_t n_mod2 = (seq_t)(n % 2);
    seq_t num2 = (nseq + 9) * n_mod2 * 63;

    /* third term: (n*((n^2) mod 3))/18
     *  multiplied by 1512/18 = 84 */
    seq_t n2_mod3 = (nseq * nseq) % 3;
    seq_t num3 = nseq * n2_mod3 * 84;

    /* combine and divide by the common denominator */
    seq_t numer = num1 - num2 - num3;
    seq_t res = numer / 1512;

    return res;
}
