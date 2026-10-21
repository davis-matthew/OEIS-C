/*
 * A073351  a(n) = n^2*(n + 1)*(2*n + 1)^2*(7*n + 1)/36.
 * https://oeis.org/A073351
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2*(n + 1)*(2*n + 1)^2*(7*n + 1)/36
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 41, u64 n <= 1694,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    __int128 x = (__int128)n;
    __int128 t = x * x;             // n^2
    t *= (x + 1);                   // * (n+1)
    t *= (2 * x + 1);               // * (2n+1)
    t *= (2 * x + 1);               // * (2n+1) again
    t *= (7 * x + 1);               // * (7n+1)
    t /= 36;                        // divide by 36
    return (seq_t)t;
}
