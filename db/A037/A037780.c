/*
 * A037780  Base 5 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 3,1,2,0.
 * https://oeis.org/A037780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 3; a(n) = 5*a(n-1) + d[n], where d[n] cycles 3, 1, 2, 0. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* behaviour for n = 0 is unused, but safe */
    static const seq_t period[4] = {3, 1, 2, 0};
    seq_t result = 0;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t digit = period[(i - 1) & 3];  /* efficient mod 4 */
        result = result * 5 + digit;
    }
    return result;
}
