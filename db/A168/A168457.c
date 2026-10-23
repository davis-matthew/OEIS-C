/*
 * A168457  a(n) = (10*n + 5*(-1)^n - 1)/2.
 * https://oeis.org/A168457
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 2 + 10*floor(n/2)
 * Range:     u8 n <= 51, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence starts at n = 1; for n = 0 return 0 as a defensive value. */
    if (n == 0)
        return 0;
    seq_t half = (seq_t)(n >> 1);          /* floor(n/2) */
    return (seq_t)2 + half * (seq_t)10;    /* 2 + 10*floor(n/2) */
}
