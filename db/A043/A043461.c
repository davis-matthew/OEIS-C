/*
 * A043461  Numbers having one 2 in base 9.
 * https://oeis.org/A043461
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: iterate natural numbers, count those with exactly one digit 2 in base 9. */
seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t x = 1; ; ++x) {
        idx_t y = x;
        unsigned int twos = 0;
        while (y > 0) {
            unsigned int d = y % 9;
            if (d == 2) {
                ++twos;
                if (twos > 1) break;
            }
            y /= 9;
        }
        if (twos == 1) {
            ++count;
            if (count == n) return (seq_t)x;
        }
    }
}
