/*
 * A047926  a(n) = (3^(n+1) + 2*n + 1)/4.
 * https://oeis.org/A047926
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3^(n+1) + 2*n + 1)/4
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent recurrence: a(0)=1, a(n+1)=3*a(n)-n */

seq_t a(idx_t n)
{
    seq_t value = 1;            /* a(0) */
    for (idx_t k = 0; k < n; ++k)
    {
        value = (seq_t)3 * value - (seq_t)k;  /* a(k+1) = 3*a(k) - k */
    }
    return value;
}
