/*
 * A037772  Base 4 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 3,1,0,2.
 * https://oeis.org/A037772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the integer whose base‑4 digits are the first n terms of
 *            the periodic sequence 3, 1, 0, 2 (repeating)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a computation: a(n) = a(n‑1) * 4 + digit[n‑1] with digit[i] = {3,1,0,2}[i mod 4] */
seq_t a(idx_t n)
{
    static const seq_t period[4] = {3, 1, 0, 2};
    seq_t ans = 0;
    for (idx_t i = 0; i < n; ++i) {
        ans = ans * 4 + period[i & 3];
    }
    return ans;
}
