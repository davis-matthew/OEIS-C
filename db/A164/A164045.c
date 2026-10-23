/*
 * A164045  a(n+1) = 5*a(n) - n.
 * https://oeis.org/A164045
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - n, with a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)1;

    seq_t val = (seq_t)1;
    /* Compute a(1) .. a(n) using the recurrence a(k) = 5*a(k-1) - k */
    for (idx_t k = 1u; k <= n; ++k)
    {
        val = val * (seq_t)5 - (seq_t)k;
    }
    return val;
}
