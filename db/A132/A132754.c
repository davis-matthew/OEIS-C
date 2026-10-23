/*
 * A132754  a(n) = n*(n + 23)/2.
 * https://oeis.org/A132754
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+23)/2
 * Range:     u8 n <= 13, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to seq_t for arithmetic. */
    seq_t a = (seq_t)n;
    seq_t b = a + (seq_t)23;

    /* Divide before multiplying to avoid overflow. */
    if ((a & 1) == 0) {
        /* a is even */
        return (a / 2) * b;
    } else {
        /* a is odd */
        return a * (b / 2) + (b & 1);
    }
}
