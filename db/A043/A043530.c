/*
 * A043530  Number of distinct base-3 digits of n.
 * https://oeis.org/A043530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of distinct base-3 digits of n
 * Range:     u8 n <= 9838, u16 n <= 9838, u32 n <= 9838, u64 n <= 9838,
 *            u128 n <= 9838, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t tmp = n;            /* n is at least 1 (OEIS offset 1)               */
    unsigned mask = 0;        /* bit i set iff digit i appears (0–2)           */

    while (tmp)
    {
        unsigned d = tmp % 3;   /* current base-3 digit (0,1,2)                 */
        mask |= 1u << d;        /* record its presence                         */
        tmp /= 3;               /* move to next digit                          */
    }

    /* popcount for a 3‑bit mask: 0..7 */
    static const unsigned bits[8] = {0,1,1,2,1,2,2,3};
    return (seq_t)bits[mask];
}
