/*
 * A037672  Base 9 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 3,2,0.
 * https://oeis.org/A037672
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Base-9 digits are periodic 3,2,0.
   a(1) = 3.
   For n > 1: a(n) = 9 * a(n-1) + d(n),
   where d(n) = 3, 2, 0, repeating every 3 terms. */
seq_t a(idx_t n)
{
    seq_t value = 0;
    for (idx_t i = 1; i <= n; ++i)
    {
        int d;
        switch ((i - 1) % 3)
        {
            case 0: d = 3; break;
            case 1: d = 2; break;
            default: d = 0; break;
        }
        value = value * 9 + d;
    }
    return value;
}
