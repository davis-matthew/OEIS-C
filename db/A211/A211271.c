/*
 * A211271  Number of integer pairs (x,y) such that 0<x<=y<=n and x*y=3n.
 * https://oeis.org/A211271
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer pairs (x,y) such that 0<x≤y≤n and x*y =
 *            3n
 * Range:     u8 n <= 85, u16 n <= 9409, u32 n <= 9409, u64 n <= 9409,
 *            u128 n <= 9409, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* defensive, offset is 1 */
    seq_t n_seq = (seq_t)n;
    seq_t m = (seq_t)3 * n_seq;      /* m = 3n */
    seq_t count = 0;
    for (seq_t x = 1; x <= n_seq; ++x)
    {
        if (m % x == 0)              /* x divides 3n */
        {
            seq_t y = m / x;
            if (y >= x && y <= n_seq)
                ++count;
        }
    }
    return count;
}
