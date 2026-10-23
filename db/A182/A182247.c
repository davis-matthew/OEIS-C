/*
 * A182247  a(0)=0, a(n) = (a(n-1) + n) OR n.
 * https://oeis.org/A182247
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 298, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0; a(n) = (a(n-1) + n) | n */

seq_t a(idx_t n)
{
    seq_t value = (seq_t)0;          /* a(0) = 0 */
    for (idx_t i = 1; i <= n; ++i)   /* cumulate up to n */
    {
        seq_t add = (seq_t)i;        /* cast index to seq_t */
        value = (value + add) | add;
    }
    return value;
}
