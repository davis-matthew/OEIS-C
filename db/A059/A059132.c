/*
 * A059132  A hierarchical sequence (W2{2}c - see A059126).
 * https://oeis.org/A059132
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(2n) = 4; a(2n + 1) = a(n) + 6
 * Range:     u8 n <= 9186, u16 n <= 9186, u32 n <= 9186, u64 n <= 9186,
 *            u128 n <= 9186, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* If n is even, the value is always 4. */
    if ((n & 1u) == 0u)
        return (seq_t)4;

    /* For odd n, count how many successive divisions by 2 keep the number odd.
       Each such division adds 6 to the result. */
    idx_t t = n;
    idx_t cnt = 0u;
    while ((t & 1u) != 0u)
    {
        t >>= 1;
        ++cnt;
    }
    return (seq_t)4 + (seq_t)6 * cnt;
}
