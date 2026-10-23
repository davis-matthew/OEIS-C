/*
 * A011911  a(n) = floor( n*(n-1)*(n-2)/29 ).
 * https://oeis.org/A011911
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( n*(n-1)*(n-2)/29 )
 * Range:     u8 n <= 7, u16 n <= 41, u32 n <= 1720, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return 0 for n < 3 because the product is zero. */
    if (n < 3) {
        return 0;
    }
    /* Compute the product using seq_t to avoid overflow in intermediate steps. */
    seq_t nn = (seq_t)n;
    seq_t prod = nn * (nn - 1) * (nn - 2);
    /* Integer division gives the floor for non‑negative integers. */
    return prod / 29;
}
