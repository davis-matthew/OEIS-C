/*
 * A055776  a(n) = a(n-1)^3 + a(n-1)^2 + a(n-1) + 1.
 * https://oeis.org/A055776
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0, a(n) = a(n-1)^3 + a(n-1)^2 + a(n-1) + 1 (n ≥ 1)
 *            (Recursive definition, implemented iteratively.)
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t cur = (seq_t)0;          /* a(0) */
    if (n == 0) return cur;

    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t t2 = cur * cur;                    /* a(i-1)^2            */
        seq_t t3 = t2 * cur;                     /* a(i-1)^3            */
        cur   = t3 + t2 + cur + (seq_t)1;        /* a(i)                */
    }
    return cur;
}
