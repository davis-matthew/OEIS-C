/*
 * A235331  Numbers with odious squares.
 * https://oeis.org/A235331
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n‑th integer whose square has an odd number of 1's
 *            in its binary representation (i.e., the n‑th odious square root)
 * Range:     u8 n <= 10, u16 n <= 386, u32 n <= 2600, u64 n <= 2600,
 *            u128 n <= 2600, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset safety */
    seq_t count = 0;
    seq_t x = 0;
    for (;;) {
        ++x;
        seq_t sq = x * x;
        seq_t t = sq;
        int popcnt = 0;
        while (t) {
            popcnt += (int)(t & 1);
            t >>= 1;
        }
        if (popcnt & 1) {
            ++count;
            if (count == n) return x;
        }
    }
}
