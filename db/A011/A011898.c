/*
 * A011898  a(n) = floor(n*(n-1)*(n-2)/16).
 * https://oeis.org/A011898
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(n-1)*(n-2)/16)
 * Range:     u8 n <= 7, u16 n <= 41, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(n) = 0 for n < 3, since the product is < 16 */
    if (n < 3)
        return (seq_t)0;

    seq_t nn = (seq_t)n;
    seq_t n1 = (seq_t)(n - 1);
    seq_t n2 = (seq_t)(n - 2);

    seq_t prod = nn * n1;
    prod = prod * n2;
    return prod / (seq_t)16;
}
