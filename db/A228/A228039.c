/*
 * A228039  Thue-Morse sequence along the squares: A010060(n^2).
 * https://oeis.org/A228039
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = s2(n^2) mod 2 (Thue-Morse value at n^2)
 * Range:     u8 n <= 15, u16 n <= 2390, u32 n <= 9728, u64 n <= 9728,
 *            u128 n <= 9728, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sq = (seq_t)n;
    sq *= sq;                      /* compute n^2 in seq_t      */
    int parity = 0;
    seq_t v = sq;
    while (v) {                    /* parity of popcount of n^2 */
        parity ^= (int)(v & 1);
        v >>= 1;
    }
    return (seq_t)parity;
}
