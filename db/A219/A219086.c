/*
 * A219086  a(n) = floor((n + 1/2)^4).
 * https://oeis.org/A219086
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n^4 + 4*n^3 + 3*n^2 + n)/2
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 251, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is equivalent to a(n) = floor((n + 1/2)^4). */
seq_t a(idx_t n)
{
    seq_t nn  = (seq_t)n;          /* n */
    seq_t n2  = nn * nn;           /* n^2 */
    seq_t n3  = n2 * nn;           /* n^3 */
    seq_t n4  = n3 * nn;           /* n^4 */

    /* numerator = 2*n^4 + 4*n^3 + 3*n^2 + n */
    seq_t numer = (seq_t)2 * n4 + (seq_t)4 * n3 + (seq_t)3 * n2 + nn;

    return numer / 2;
}
