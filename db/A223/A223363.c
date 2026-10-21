/*
 * A223363  6 X 6 X 6 triangular graph coloring a rectangular array: number of
 *          n X 1 0..20 arrays where 0..20 label nodes of the fully
 *          triangulated graph and every array movement to a horizontal or
 *          vertical neighbor moves along an edge of this graph.
 * https://oeis.org/A223363
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 4*a(n-2) - 8*a(n-3) for n>=4 with initial
 *            values: a(1)=21, a(2)=90, a(3)=420
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0; /* undefined for n<1, but return 0 safely */
    }
    switch (n)
    {
        case 1: return 21;
        case 2: return 90;
        case 3: return 420;
    }

    seq_t a0 = 21;   /* a(n-3) */
    seq_t a1 = 90;   /* a(n-2) */
    seq_t a2 = 420;  /* a(n-1) */
    seq_t a_next;

    for (idx_t i = (idx_t)4; i <= n; ++i)
    {
        a_next = (seq_t)6 * a2 - (seq_t)4 * a1 - (seq_t)8 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a_next;
    }
    return a2;
}
