/*
 * A016987  a(n) = (7*n)^7.
 * https://oeis.org/A016987
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*n)^7
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 3, u64 n <= 80, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute (7*n)^7 using only integer multiplication.
       The sequence can be expressed as 823543 * n^7, but
       we perform the calculation directly to avoid extra
       intermediate large constants. */
    seq_t m  = (seq_t)n * (seq_t)7;    /* m = 7*n   */
    seq_t m2 = m * m;                  /* m^2       */
    seq_t m4 = m2 * m2;                /* m^4       */
    seq_t m6 = m4 * m2;                /* m^6       */
    seq_t res = m6 * m;                /* m^7       */
    return res;
}
