/*
 * A160381  Number of 1's in base-4 representation of n.
 * https://oeis.org/A160381
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of 1's in base-4 representation of n
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t count = 0;
    idx_t x = n;
    while (x) {
        if ((x & 3) == 1)
            count++;
        x >>= 2;
    }
    return count;
}
