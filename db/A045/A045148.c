/*
 * A045148  Numbers whose base-4 representation contains exactly three 2's and
 *          no 3's.
 * https://oeis.org/A045148
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A045148: numbers whose base‑4 representation contains exactly three 2’s and no 3’s */
seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t x = 1; ; ++x) {
        idx_t y = x;
        int twos = 0;
        int bad = 0;
        while (y) {
            int d = y & 3;          /* y % 4 */
            if (d == 3) { bad = 1; break; }
            if (d == 2) { ++twos; if (twos > 3) { bad = 1; break; } }
            y >>= 2;                /* y /= 4 */
        }
        if (!bad && twos == 3) {
            ++count;
            if (count == n) {
                return (seq_t)x;
            }
        }
    }
}
