/*
 * A037481  Base 4 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,2.
 * https://oeis.org/A037481
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 4*a(n-1) + a(n-2) - 4*a(n-3); a(0) = 0, a(1) = 1, a(2) =
 *            6
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)6;

    seq_t a0 = (seq_t)0;
    seq_t a1 = (seq_t)1;
    seq_t a2 = (seq_t)6;

    for (idx_t i = 3; i <= n; ++i) {
        seq_t next = (seq_t)4 * a2 + a1 - (seq_t)4 * a0;
        a0 = a1;
        a1 = a2;
        a2 = next;
    }
    return a2;
}
