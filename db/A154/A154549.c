/*
 * A154549  a(n) = 111111*n.
 * https://oeis.org/A154549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 111111*n
 * Range:     u8 none, u16 none, u32 n <= 1000, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (also satisfies a(n) = a(n-1) + 111111 for n > 1) */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;        /* 0 for out-of-range indices, no effect on valid n≥1 */
    return (seq_t)n * 111111;   /* direct product, works for any seq_t size */
}
