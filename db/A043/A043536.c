/*
 * A043536  Number of distinct base-9 digits of n.
 * https://oeis.org/A043536
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of distinct base-9 digits of n
 * Range:     u8 n <= 66430, u16 n <= 66430, u32 n <= 66430, u64 n <= 66430,
 *            u128 n <= 66430, big any n
 * Validated: standard, pass, 121 terms through n = 9862 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    unsigned int mask = 0;
    idx_t x = n;
    while (x != 0) {
        unsigned int digit = (unsigned int)(x % 9);
        mask |= 1u << digit;
        x /= 9;
    }
    int count = 0;
    while (mask) {
        count++;
        mask &= mask - 1;
    }
    return (seq_t)count;
}
