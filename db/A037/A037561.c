/*
 * A037561  Base 4 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,2,1.
 * https://oeis.org/A037561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=2; a(n)=4*a(n-1)+1 if n%3==0 else +2 */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t val = 0;
    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t digit = (i % 3 == 0) ? (seq_t)1 : (seq_t)2;
        val = val * 4 + digit;
    }
    return val;
}
