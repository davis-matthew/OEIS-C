/*
 * A199412  a(n) = (9*6^n+1)/5.
 * https://oeis.org/A199412
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 1, a(0) = 2
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)2;

    seq_t res = (seq_t)2;
    /* Compute the recurrence iteratively; works for any
       unsigned seq_t (8,16,32,64,128 bits) or arbitrary‑precision type. */
    for (idx_t i = 1; i <= n; ++i) {
        res = res * (seq_t)6 - (seq_t)1;
    }
    return res;
}
