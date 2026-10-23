/*
 * A043537  Number of distinct base-10 digits of n.
 * https://oeis.org/A043537
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of distinct base-10 digits of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;           /* undefined for offset 0, but safe */

    idx_t x = n;
    unsigned int mask = 0;   /* bitmask of the digits seen */

    while (x > 0) {
        unsigned int d = x % 10;
        mask |= (1u << d);
        x /= 10;
    }

    /* count the set bits in mask: number of distinct digits */
    seq_t cnt = 0;
    while (mask) {
        cnt++;
        mask &= mask - 1;
    }

    return cnt;
}
