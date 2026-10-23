/*
 * A168376  a(n) = (14*n - 7*(-1)^n - 9)/4.
 * https://oeis.org/A168376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3 + 7*floor((n-1)/2)
 * Range:     u8 n <= 74, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute floor((n-1)/2) safely in idx_t, then cast to seq_t */
    seq_t m = (seq_t)((n - 1) / 2);
    return 3 + 7 * m;
}
