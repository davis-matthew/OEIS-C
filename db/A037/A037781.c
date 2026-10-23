/*
 * A037781  Base 6 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 3,1,2,0.
 * https://oeis.org/A037781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 3.
   For n > 1, a(n) = a(n-1) * 6 + pattern[(n-1) mod 4],
   where pattern = {3, 1, 2, 0}. */
seq_t a(idx_t n)
{
    static const unsigned char pattern[4] = {3, 1, 2, 0};
    seq_t val = (seq_t)3;

    if (n <= 1) return val;

    for (idx_t i = 1; i < n; ++i) {
        val = val * (seq_t)6 + (seq_t)pattern[i & 3];
    }
    return val;
}
