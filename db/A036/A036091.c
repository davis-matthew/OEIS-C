/*
 * A036091  Centered cube numbers: (n+1)^13+n^13.
 * https://oeis.org/A036091
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n + 1)^13 + n^13
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 28, u128 n <= 872,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow13(seq_t x) {
    seq_t r = 1;
    for (int i = 0; i < 13; ++i) {
        r *= x;
    }
    return r;
}

seq_t a(idx_t n) {
    /* Convert the index to the sequence type */
    seq_t x = (seq_t)n;

    /* Compute (n + 1)^13 and n^13 */
    seq_t p1 = pow13(x + 1);  /* (n + 1)^13 */
    seq_t p2 = pow13(x);      /*  n^13   */

    /* Return their sum */
    return p1 + p2;
}
