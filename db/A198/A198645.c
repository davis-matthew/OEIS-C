/*
 * A198645  a(n) = 10*3^n - 1.
 * https://oeis.org/A198645
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*3^n - 1
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 18, u64 n <= 38, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 3^n by exponentiation by squaring */
    seq_t power = 1;          /* 3^0 = 1 */
    seq_t base  = (seq_t)3;   /* base = 3 */
    idx_t exp   = n;

    while (exp > 0) {
        if (exp & 1)
            power *= base;    /* multiply when current bit is set */
        base *= base;         /* square the base */
        exp >>= 1;            /* shift to the next bit */
    }

    /* Now power = 3^n, so compute 10*3^n - 1 */
    power = power * (seq_t)10 - (seq_t)1;
    return power;
}
