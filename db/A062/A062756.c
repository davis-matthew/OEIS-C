/*
 * A062756  Number of 1's in ternary (base-3) expansion of n.
 * https://oeis.org/A062756
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of 1's in the base-3 representation of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t count = 0;
    idx_t cur = n;
    while (cur != 0) {
        if ((cur % 3) == 1)
            count++;
        cur = cur / 3;
    }
    return count;
}
