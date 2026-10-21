/*
 * A223417  3-level binary fanout graph coloring a rectangular array: number
 *          of nX1 0..6 arrays where 0..6 label nodes of a graph with edges
 *          0,1 1,3 1,4 0,2 2,5 2,6 and every array movement to a horizontal
 *          or vertical neighbor moves along an edge of this graph.
 * https://oeis.org/A223417
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-2) for n>3, a(1)=7, a(2)=12, a(3)=26
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 1) return (seq_t)7;
    if (n == 2) return (seq_t)12;
    if (n == 3) return (seq_t)26;

    /* For n >= 4 use the recurrence, iterated in O(n) time */
    seq_t result;
    if ((n & 1) == 0) {              /* n even */
        result = (seq_t)48;          /* a(4) */
        idx_t exp = (n - 4) / 2;     /* number of times to multiply by 4 */
        for (idx_t i = 0; i < exp; ++i)
            result *= (seq_t)4;
    } else {                         /* n odd (n >= 5) */
        result = (seq_t)26;          /* a(3) */
        idx_t exp = (n - 3) / 2;
        for (idx_t i = 0; i < exp; ++i)
            result *= (seq_t)4;
    }
    return result;
}
