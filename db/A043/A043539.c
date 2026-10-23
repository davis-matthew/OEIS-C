/*
 * A043539  Number of distinct base-12 digits of n.
 * https://oeis.org/A043539
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of distinct digits in the base‑12 representation
 *            of n
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* mask for the 12 possible digits 0–11 */
    unsigned int mask = 0;
    idx_t m = n;           /* use a local copy since n is read-only */
    while (m > 0) {
        unsigned int d = (unsigned int)(m % 12);
        mask |= 1u << d;
        m /= 12;
    }
    /* Count set bits in mask (number of distinct digits) */
    unsigned int count = 0;
    while (mask) {
        mask &= mask - 1;  /* clear lowest set bit */
        count++;
    }
    return (seq_t)count;
}
