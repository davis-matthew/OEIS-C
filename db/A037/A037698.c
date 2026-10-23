/*
 * A037698  Base 7 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,2,3,0.
 * https://oeis.org/A037698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1; a(n) = 7*a(n-1) + d(n) for n ≥ 2,
   where d(n) cycles through 1, 2, 3, 0 (i.e., d(n) = {1,2,3,0}[(n-1) mod 4]) */

seq_t a(idx_t n)
{
    if (n <= 1)
        return (n == 1) ? (seq_t)1 : (seq_t)0;

    static const unsigned char pattern[4] = {1, 2, 3, 0};
    seq_t val = (seq_t)1;   /* a(1) */
    for (idx_t i = 2; i <= n; ++i)
    {
        unsigned int idx = (unsigned int)((i - 1) % 4);
        val = 7 * val + (seq_t)pattern[idx];
    }
    return val;
}
