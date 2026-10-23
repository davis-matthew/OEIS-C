/*
 * A011195  a(n) = n*(n+1)*(2*n+1)*(3*n+1)/6.
 * https://oeis.org/A011195
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(2*n+1)*(3*n+1)/6
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 255, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* work in seq_t to avoid overflow in intermediate steps */
    seq_t nn = (seq_t)n;          /* n */
    seq_t n1 = nn + 1;            /* n + 1 */

    /* n*(n+1) / 2  – one factor is even, division is exact */
    seq_t t = nn * n1;
    t /= 2;

    /* multiply by (2*n + 1) and divide by 3 – product is always divisible by 3 */
    seq_t two_n_plus1 = 2 * nn + 1;
    t *= two_n_plus1;
    t /= 3;

    /* remaining factor (3*n + 1) */
    seq_t three_n_plus1 = 3 * nn + 1;
    t *= three_n_plus1;

    return t;
}
