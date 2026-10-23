/*
 * A043442  Numbers having two 5's in base 8.
 * https://oeis.org/A043442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth positive integer whose base‑8 (octal)
 *            representation contains exactly two digits 5
 * Range:     u8 n <= 4, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t num   = 0;
    idx_t count = 0;
    while (1) {
        /* Count the number of 5's in the octal representation of num. */
        idx_t x = num;
        int c   = 0;
        while (x) {
            if (x % 8 == 5) {
                c++;
                if (c > 2)
                    break;
            }
            x /= 8;
        }
        if (c == 2) {
            count++;
            if (count == n)
                return (seq_t)num;
        }
        num++;
    }
}
