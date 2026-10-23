/*
 * A134227  Row sums of triangle A134226.
 * https://oeis.org/A134227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1)*(n+6)/2 + [n==1]
 * Range:     u8 n <= 20, u16 n <= 359, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Offset is 1; values for n<1 are undefined, but we return 0 for safety. */
    if (n == 0) return 0;
    if (n == 1) return 1;

    /* Compute using seq_t arithmetic to avoid overflow in intermediate 64‑bit
       unsigned values. */
    seq_t nn = (seq_t)n;
    return ((nn - 1) * (nn + 6)) / 2;
}
