/*
 * A074232  Positive numbers that are not 3 or 6 (mod 9).
 * https://oeis.org/A074232
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*⌊(n-1)/7⌋ + {1,2,4,5,7,8,9}[(n-1)%7]
 * Range:     u8 n <= 198, u16 n <= 9880, u32 n <= 9880, u64 n <= 9880,
 *            u128 n <= 9880, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for n<1, handle gracefully */
    idx_t block   = (n - 1) / 7;   /* number of complete 9‑number blocks */
    idx_t offset  = (n - 1) % 7;   /* position within the block (0…6) */
    static const seq_t map[7] = {1, 2, 4, 5, 7, 8, 9};
    return (seq_t)block * 9 + map[offset];
}
