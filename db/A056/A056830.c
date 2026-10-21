/*
 * A056830  Alternate digits 1 and 0.
 * https://oeis.org/A056830
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 10*a(n-1) + (1 - (-1)^n)/2 , a(0) = 0
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS sequence A056830 (alternate digits 1 and 0). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    seq_t res = 0;
    /* Iterate from 1 to n, adding 1 at odd positions (digits '1')
       and 0 at even positions (digits '0'). */
    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t add = (i & (idx_t)1) ? (seq_t)1 : (seq_t)0;
        res = (seq_t)10 * res + add;
    }
    return res;
}
