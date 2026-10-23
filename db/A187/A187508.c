/*
 * A187508  Number of 3-step S, E, and NW-moving king's tours on an n X n
 *          board summed over all starting positions.
 * https://oeis.org/A187508
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 0; for n > 1, a(n) = 9*n*n - 20*n + 10
 * Range:     u8 n <= 6, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;
    seq_t sn = (seq_t)n;
    return sn * sn * (seq_t)9 - sn * (seq_t)20 + (seq_t)10;
}
