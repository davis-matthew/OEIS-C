/*
 * A037711  Base 6 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,3,2,0.
 * https://oeis.org/A037711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A037711: Base‑6 digits are 1,3,2,0 repeating.
   a(n) is the number whose first n base‑6 digits follow the period
   1,3,2,0.  The value is built as val = val*6 + next digit. */
seq_t a(idx_t n)
{
    static const seq_t period[4] = {1, 3, 2, 0};
    seq_t val = 0;
    for (idx_t i = 1; i <= n; ++i) {
        val = val * 6 + period[(i - 1) & 3];
    }
    return val;
}
