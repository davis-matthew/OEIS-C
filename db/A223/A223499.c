/*
 * A223499  Petersen graph (3,1) coloring a rectangular array: number of n X 3
 *          0..5 arrays where 0..5 label nodes of a graph with edges 0,1 0,3
 *          3,5 3,4 1,2 1,4 4,5 2,0 2,5 and every array movement to a
 *          horizontal, diagonal or antidiagonal neighbor moves along an edge
 *          of this graph, with the array starting at 0.
 * https://oeis.org/A223499
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 15*a(n-1) - 24*a(n-2) + 10*a(n-3). Initial values: a(1) =
 *            9, a(2) = 115, a(3) = 1519
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n == 0 is out of bounds for the sequence; return 0 as a harmless default. */
    if (n == 0) return (seq_t)0;

    /* Handle the first three values explicitly. */
    if (n == 1) return (seq_t)9;
    if (n == 2) return (seq_t)115;
    if (n == 3) return (seq_t)1519;

    /* Set up a rolling window for the last three terms:
         a1 = a(n-1), a2 = a(n-2), a3 = a(n-3). */
    seq_t a1 = (seq_t)1519;   /* a(3) */
    seq_t a2 = (seq_t)115;    /* a(2) */
    seq_t a3 = (seq_t)9;      /* a(1) */

    /* Iterate until we reach the desired index n. */
    for (idx_t i = 4; i <= n; ++i)
    {
        /* next = 15*a1 - 24*a2 + 10*a3 */
        seq_t next = (seq_t)15 * a1 - (seq_t)24 * a2 + (seq_t)10 * a3;

        /* Shift the window for the next iteration. */
        a3 = a2;
        a2 = a1;
        a1 = next;
    }

    /* Now a1 holds a(n). */
    return a1;
}
