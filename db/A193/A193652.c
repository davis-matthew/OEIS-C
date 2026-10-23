/*
 * A193652  A020988 and A007583 interleaved.
 * https://oeis.org/A193652
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 * (4^floor(n/2) - 1) / 3 + n mod 2
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 33, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 4^(floor(n/2)) */
    idx_t k = n >> 1;                       /* floor(n/2) */
    seq_t pow4 = (seq_t)1;
    seq_t base = (seq_t)4;
    idx_t exp = k;
    while (exp) {
        if (exp & 1)
            pow4 *= base;
        exp >>= 1;
        if (exp)
            base *= base;
    }

    seq_t tmp = (pow4 - (seq_t)1) / (seq_t)3;   /* (4^k - 1)/3  */
    tmp *= (seq_t)2;                            /* times 2     */
    if (n & 1)
        ++tmp;                                  /* add 1 if odd */

    return tmp;
}
