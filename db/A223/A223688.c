/*
 * A223688  Petersen graph (8,2) coloring a rectangular array: number of n X 4
 *          0..15 arrays where 0..15 label nodes of a graph with edges 0,1 0,8
 *          8,14 8,10 1,2 1,9 9,15 9,11 2,3 2,10 10,12 3,4 3,11 11,13 4,5 4,12
 *          12,14 5,6 5,13 13,15 6,7 6,14 7,0 7,15 and every array movement to
 *          a horizontal or antidiagonal...
 * https://oeis.org/A223688
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 5, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 59*a(n-1) - 1103*a(n-2) + 7621*a(n-3) - 16900*a(n-4)
 * Initial values: a(1)=432, a(2)=12384, a(3)=363600, a(4)=10817856
 */
seq_t a(idx_t n)
{
    /* handle offset 1 */
    if (n == 0) return 0;
    if (n == 1) return 432;
    if (n == 2) return 12384;
    if (n == 3) return 363600;
    if (n == 4) return 10817856;

    /* last four terms */
    seq_t a1 = 432;          /* a(n-4)  */
    seq_t a2 = 12384;        /* a(n-3)  */
    seq_t a3 = 363600;       /* a(n-2)  */
    seq_t a4 = 10817856;     /* a(n-1)  */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t tmp = 59 * a4 - 1103 * a3 + 7621 * a2 - 16900 * a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = tmp;
    }
    return a4;
}
