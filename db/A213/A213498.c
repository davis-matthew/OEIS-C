/*
 * A213498  Number of (w,x,y) with all terms in {0,...,n} and w !=
 *          max(|w-x|,|x-y|,|y-w|).
 * https://oeis.org/A213498
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n*(4*n^2+5*n+5) - (2*n+1)*(-1)^n + 1)/8, a(0)=0
 * Range:     u8 n <= 2, u16 n <= 19, u32 n <= 939, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    /* term1 = 2*n*(4*n^2+5*n+5) */
    seq_t term1 = (seq_t)2 * nn *
                  ((seq_t)4 * nn * nn + (seq_t)5 * nn + (seq_t)5);
    seq_t num = term1 + 1;            /* add the +1 in numerator */
    if ((n & 1) == 0) {               /* even n: subtract (2*n+1) */
        num -= (seq_t)(2 * n + 1);
    } else {                          /* odd n: add (2*n+1) */
        num += (seq_t)(2 * n + 1);
    }
    return num / 8;                    /* final division by 8 */
}
