/*
 * A037769  Base 8 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 3,0,2,1.
 * https://oeis.org/A037769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 3.
   a(n+1) = 8 * a(n) + d(n+1),   where d is the periodic digit
   sequence 3,0,2,1 repeated forever.  */
seq_t a(idx_t n)
{
    const unsigned char pattern[4] = {3, 0, 2, 1};
    seq_t result = 0;
    for (idx_t i = 0; i < n; ++i) {
        unsigned char d = pattern[i & 3];   /* i % 4, 0‑based */
        result *= 8;
        result += (seq_t)d;
    }
    return result;
}
