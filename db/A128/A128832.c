/*
 * A128832  Number of n-tuples where each entry is chosen from the subsets of
 *          {1,2,3,4} such that the intersection of all n entries is empty.
 * https://oeis.org/A128832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2^n - 1)^4
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute 2^n by repeated squaring */
    seq_t pow2 = (seq_t)1;        /* running product */
    seq_t base  = (seq_t)2;        /* base */
    idx_t exp   = n;               /* exponent */

    while (exp > 0) {
        if (exp & 1) pow2 *= base;
        base *= base;
        exp >>= 1;
    }

    /* (2^n - 1) */
    seq_t tmp = pow2 - (seq_t)1;

    /* square twice: ((2^n - 1)^2)^2 = (2^n - 1)^4 */
    tmp = tmp * tmp;   /* square once */
    tmp = tmp * tmp;   /* square again */

    return tmp;
}
