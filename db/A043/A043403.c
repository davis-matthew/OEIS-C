/*
 * A043403  Numbers whose base-7 representation contains exactly three 2's.
 * https://oeis.org/A043403
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th smallest positive integer whose base‑7
 *            representation contains exactly three '2' digits
 * Range:     u8 n <= 1, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation searches integers sequentially and counts base‑7 digits. */
seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t x = 0;
    while (count < n) {
        x = x + 1;
        idx_t twos = 0;
        seq_t y = x;
        while (y != 0) {
            seq_t digit = y % (seq_t)7;
            if (digit == (seq_t)2) {
                ++twos;
                if (twos > 3)
                    break;
            }
            y = y / (seq_t)7;
        }
        if (twos == 3)
            ++count;
    }
    return x;
}
