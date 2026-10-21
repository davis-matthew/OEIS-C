/*
 * A223290  4-loop graph coloring a rectangular array: number of n X 1 0..8
 *          arrays where 0..8 label nodes of a graph with edges 0,1 1,2 2,0
 *          0,3 3,4 4,0 0,5 5,6 6,0 0,7 7,8 8,0 and every array movement to a
 *          horizontal or vertical neighbor moves along an edge of this graph.
 * https://oeis.org/A223290
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 8*a(n-2) Initial values: a(1)=9, a(2)=24
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 17, u64 n <= 35, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)9;
    if (n == 2) return (seq_t)24;

    seq_t a_im1 = (seq_t)24; /* a(2) */
    seq_t a_im2 = (seq_t)9;  /* a(1) */
    seq_t next;

    for (idx_t i = 3; i <= n; ++i) {
        next = a_im1 + (seq_t)8 * a_im2;
        a_im2 = a_im1;
        a_im1 = next;
    }
    return a_im1;
}
