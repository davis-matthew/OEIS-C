/*
 * A156641  a(n) = 13*(100^(n+1) - 1)/99.
 * https://oeis.org/A156641
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 13*(100^(n+1)-1)/99
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 13; a(n) = 100*a(n-1) + 13 for n >= 1 */

seq_t a(idx_t n)
{
    seq_t val = (seq_t)13;
    for (idx_t i = 0; i < n; ++i)
    {
        val = val * (seq_t)100 + (seq_t)13;
    }
    return val;
}
