/*
 * A037507  Base 6 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,2,0.
 * https://oeis.org/A037507
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1; for n > 1, a(n) = 6*a(n‑1) + digit(n),
   where digit(n) repeats 1, 2, 0 for n = 1, 2, 3, … */
seq_t a(idx_t n)
{
    static const seq_t digits[3] = {1, 2, 0};
    seq_t result = 0;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t d = digits[(i - 1) % 3];
        result = result * 6 + d;
    }
    return result;
}
