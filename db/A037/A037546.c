/*
 * A037546  Base 5 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,2,2.
 * https://oeis.org/A037546
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1. For n >= 2,
   a(n) = 5 * a(n-1) + d_{n-1},
   where d_{k} = 1 if k mod 3 == 0, otherwise 2. */

seq_t a(idx_t n)
{
    seq_t result = 0;
    for (idx_t i = 0; i < n; ++i) {
        seq_t digit = (i % 3 == 0) ? 1 : 2;
        result = result * 5 + digit;
    }
    return result;
}
