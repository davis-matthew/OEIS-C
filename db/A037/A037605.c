/*
 * A037605  Base 5 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,2,3.
 * https://oeis.org/A037605
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = base‑5 number whose first n digits are the periodic
 *            sequence 1,2,3,1,2,3,… (digits 1,2,3 repeating)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = 0;
    for (idx_t i = 1; i <= n; ++i) {
        val = val * 5 + (seq_t)(((i - 1) % 3) + 1);
    }
    return val;
}
