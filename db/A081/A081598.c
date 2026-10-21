/*
 * A081598  Let n = 10x + y where 0 <= y <= 9, x >= 0. Then a(n) = 7x+y.
 * https://oeis.org/A081598
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - 3*floor(n/10)
 * Range:     u8 n <= 363, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* That is, if n = 10*x + y with 0 <= y <= 9, then a(n) = 7*x + y. */

seq_t a(idx_t n)
{
    /* Convert the quotient and remainder to seq_t, then compute the result */
    seq_t x = (seq_t)(n / 10);
    seq_t y = (seq_t)(n % 10);
    return 7 * x + y;
}
