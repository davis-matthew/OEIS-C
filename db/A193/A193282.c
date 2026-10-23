/*
 * A193282  a(n) = (n!/floor(n/2)!)^2.
 * https://oeis.org/A193282
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n! / floor(n/2)!)^2
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) by multiplying the range (floor(n/2)+1) … n and squaring the result. */
seq_t a(idx_t n)
{
    idx_t m = n / 2;          /* floor(n/2) */
    seq_t prod = 1;           /* running product for the range m+1 .. n */

    /* For n = 0 or 1 the loop body is skipped and prod stays 1, giving a(n)=1. */
    for (idx_t i = m + 1; i <= n; ++i) {
        prod *= (seq_t)i;
    }

    return prod * prod;       /* square the product to get the final value */
}
