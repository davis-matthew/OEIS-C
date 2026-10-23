/*
 * A018839  Squares on infinite chessboard at n moves from center using a
 *          {2,3} fairy knight (or zebra).
 * https://oeis.org/A018839
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 68*n - 72 for n >= 8 a(0) = 1, a(1) = 8, a(2) = 32, a(3)
 *            = 88, a(4) = 192, a(5) = 304, a(6) = 372, a(7) = 416
 * Range:     u8 n <= 4, u16 n <= 964, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Problem: compute the number of squares reachable by a {2,3} fairy knight
   (zebra) in n moves from the center of an infinite chessboard. */

/* The sequence is linear for n >= 8, so we use a small lookup table
   for the first eight values and a simple arithmetic formula afterwards. */

seq_t a(idx_t n)
{
    /* First eight known terms. */
    static const seq_t table[8] = {
        (seq_t)1,  /* a(0) */
        (seq_t)8,  /* a(1) */
        (seq_t)32, /* a(2) */
        (seq_t)88, /* a(3) */
        (seq_t)192,/* a(4) */
        (seq_t)304,/* a(5) */
        (seq_t)372,/* a(6) */
        (seq_t)416 /* a(7) */
    };

    if (n < 8) {
        return table[n];
    } else {
        /* Compute 68*n - 72 using seq_t arithmetic to avoid overflow
           of intermediate results for the supported bit widths. */
        seq_t nn = (seq_t)n;
        return (seq_t)68 * nn - (seq_t)72;
    }
}
