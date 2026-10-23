/*
 * A037667  Base 4 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 3,2,0.
 * https://oeis.org/A037667
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number whose base‑4 digits are the first n terms of the
 *            repeating pattern 3,2,0 (period 3). Recurrence: a(1) = 3, a(n) =
 *            4*a(n-1) + digit(n) where digit(n) = 3 if (n-1)%3==0, 2 if
 *            (n-1)%3==1, 0 otherwise
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 0;
    for (idx_t i = 1; i <= n; ++i) {
        idx_t r = (i - 1) % 3;
        seq_t digit = (r == 0) ? 3
                     : (r == 1) ? 2
                     : 0;
        res = res * 4 + digit;
    }
    return res;
}
