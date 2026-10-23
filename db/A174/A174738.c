/*
 * A174738  Partial sums of floor(n/7).
 * https://oeis.org/A174738
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n-2)*(n-3)/14) (offset 0)
 * Range:     u8 n <= 18, u16 n <= 449, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n = 0..6 the expression is non‑positive; return 0 directly. */
    if (n < 7) return 0;

    seq_t un = (seq_t)n;
    seq_t num = (un - 2) * (un - 3);
    return num / 14;                     /* integer division truncates to floor */
}
