/*
 * A037755  Base 8 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,3,1,0.
 * https://oeis.org/A037755
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 2; for n > 1, a(n) = a(n-1) * 8 + d, where
   d = {2, 3, 1, 0}[ (n-1) mod 4 ]   (the repeating base‑8 digit sequence) */

seq_t a(idx_t n)
{
    seq_t val = 0;
    if (n == 0) return val;          /* safety: never used (sequence starts at n=1) */

    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t digit;
        switch ((i - 1) & 3)         /* (i-1) % 4, faster with bitwise AND */
        {
            case 0: digit = 2; break;
            case 1: digit = 3; break;
            case 2: digit = 1; break;
            default: digit = 0; break; /* case 3 */
        }
        val = val * 8 + digit;
    }

    return val;
}
