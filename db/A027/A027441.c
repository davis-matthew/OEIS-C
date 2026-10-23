/*
 * A027441  a(n) = (n^4 + n)/2 (Row sums of an n X n X n magic cube, when it
 *          exists).
 * https://oeis.org/A027441
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^4 + n)/2
 * Range:     u8 n <= 4, u16 n <= 19, u32 n <= 270, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed directly from the definition. */
seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n3 = nn * nn * nn;      /* n^3 */
    seq_t res = (nn * (n3 + 1)) / 2;
    return res;
}
