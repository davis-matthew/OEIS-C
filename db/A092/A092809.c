/*
 * A092809  Expansion of (1+x-x^2) / ((1-x^2)*(1-4*x^2)).
 * https://oeis.org/A092809
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5*2^n/6 + (-2)^n/6 + (-1)^n/6 - 1/6 a(2k) = 4^k a(2k+1) =
 *            (4*4^k - 1)/3
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if ((n & 1) == 0) {                     /* even n : n = 2*k   */
        idx_t k = n >> 1;                   /* k = n/2             */
        return ((seq_t)1) << (2 * k);       /* 1 << 2k  == 4^k     */
    } else {                                 /* odd n  : n = 2*k+1  */
        idx_t k = (n - 1) >> 1;             /* k = (n-1)/2         */
        seq_t val = (((seq_t)4) << (2 * k)) - 1; /* 4*4^k - 1          */
        return val / 3;                     /* division exact       */
    }
}
