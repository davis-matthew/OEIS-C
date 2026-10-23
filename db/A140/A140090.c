/*
 * A140090  a(n) = n*(3*n + 7)/2.
 * https://oeis.org/A140090
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*(3*n + 7)/2
 * Range:     u8 n <= 8, u16 n <= 206, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast index to seq_t to avoid intermediate overflow when seq_t is large */
    seq_t nn = (seq_t)n;
    seq_t term = nn * nn;               /* nn^2 */
    term = (seq_t)3 * term + (seq_t)7 * nn; /* 3*n^2 + 7*n */
    return term / 2;
}
