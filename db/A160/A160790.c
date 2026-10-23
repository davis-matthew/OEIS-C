/*
 * A160790  Vertex number of a rectangular spiral. The first differences
 *          (A160791) are the edge lengths of the spiral. The distances
 *          between two nearest edges, that are parallel to the initial edge,
 *          are the natural numbers.
 * https://oeis.org/A160790
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2*n+3+(-1)^n)*(2*n+3-3*(-1)^n)*(2*n+15+5*(-1)^n)/384
 * Range:     u8 n <= 19, u16 n <= 143, u32 n <= 1164, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn   = (seq_t)n;
    seq_t two  = nn * 2;
    seq_t t1, t2, t3;
    if ((n & 1) == 0) {          /* n even */
        t1 = two + 4;
        t2 = two;
        t3 = two + 20;
    } else {                     /* n odd */
        t1 = two + 2;
        t2 = two + 6;
        t3 = two + 10;
    }
    return (t1 * t2 * t3) / 384;
}
