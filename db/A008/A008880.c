/*
 * A008880  3x + 1 sequence starting at 33.
 * https://oeis.org/A008880
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 200, u16 n <= 200, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 33; a(n) = (a(n-1) & 1 ? 3*a(n-1)+1 : a(n-1)>>1) */
seq_t a(idx_t n)
{
    seq_t value = (seq_t)33;
    for (idx_t i = 0; i < n; ++i)
    {
        if ((value & (seq_t)1) == 0)
            value >>= 1;
        else
            value = value * 3 + 1;
    }
    return value;
}
