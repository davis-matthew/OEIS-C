/*
 * A087508  Number of k such that mod(k*n,3) = 1 for 0 <= k <= n.
 * https://oeis.org/A087508
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 0 if n % 3 == 0; otherwise a(n) = ((n - (n % 3)) / 3) + 1
 * Range:     u8 n <= 765, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % 3;
    if (r == 0)
        return (seq_t)0;
    /* (n - r) is always non‑negative and fits in idx_t. */
    return (seq_t)((n - r) / 3 + 1);
}
