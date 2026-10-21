/*
 * A077414  a(n) = n*(n - 1)*(n + 2)/2.
 * https://oeis.org/A077414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)*(n+2)/2
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    /* Compute n*(n-1)*(n+2)/2 using 64‑bit arithmetic of seq_t. */
    seq_t prod = nn * (nn - 1) * (nn + 2);
    return prod / 2;
}
