/*
 * A131473  a(n) = n^6 - n.
 * https://oeis.org/A131473
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^6 - n
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 40, u64 n <= 1625,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the target integer type */
    seq_t x = (seq_t)n;

    /* Use an unsigned 128‑bit integer for the intermediate
       computations so that we never overflow until the final cast. */
    unsigned __int128 t = (unsigned __int128)x;

    t *= (unsigned __int128)x; /* n^2   */
    t *= (unsigned __int128)x; /* n^3   */
    t *= (unsigned __int128)t; /* n^6   */
    t -= (unsigned __int128)x; /* n^6 - n */

    return (seq_t)t;
}
