/*
 * A038716  a(n) = floor(n/4)*ceiling((n+3)/4).
 * https://oeis.org/A038716
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n/4)*ceil((n+3)/4)
 * Range:     u8 n <= 63, u16 n <= 1023, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* floor division gives the first factor */
    seq_t q = (seq_t)(n / 4);

    /* remainder determines the second factor */
    idx_t r = n % 4;
    seq_t ceil_val = q + ((r < 2) ? (seq_t)1 : (seq_t)2);

    return q * ceil_val;
}
