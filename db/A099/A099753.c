/*
 * A099753  a(n) = (2*n+1)^(n+2).
 * https://oeis.org/A099753
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n+1)^(n+2)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base  = (seq_t)2 * (seq_t)n + 1;   /* 2*n+1 as seq_t */
    seq_t result = 1;
    idx_t exp = n + 2;                       /* exponent (n+2) */

    while (exp) {
        if (exp & 1) result *= base;         /* if LSB is 1, multiply the result */
        exp >>= 1;                           /* shift exponent right */
        if (exp) base *= base;                /* square the base for next bit */
    }
    return result;
}
