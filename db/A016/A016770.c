/*
 * A016770  a(n) = (3*n)^6.
 * https://oeis.org/A016770
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n)^6
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 13, u64 n <= 541, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t ns   = (seq_t)n;  /* convert index to seq_t */
    seq_t base = ns * 3;    /* base = 3*n */

    seq_t res = 1;          /* initialize result as 1 */
    /* Multiply the base 6 times to compute (3*n)^6 */
    res *= base;
    res *= base;
    res *= base;
    res *= base;
    res *= base;
    res *= base;

    return res;
}
