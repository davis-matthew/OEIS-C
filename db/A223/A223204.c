/*
 * A223204  Rolling icosahedron face footprints: number of n X 3 0..19 arrays
 *          starting with 0 where 0..19 label faces of an icosahedron and
 *          every array movement to a horizontal or vertical neighbor moves
 *          across an icosahedral edge.
 * https://oeis.org/A223204
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) - 2*a(n-2) (a(1)=9, a(2)=75)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle requested range */
    if (n == 0) return (seq_t)0;          /* Not part of the OEIS sequence */
    if (n == 1) return (seq_t)9;
    if (n == 2) return (seq_t)75;

    seq_t a_prev2 = (seq_t)9;   /* a(1) */
    seq_t a_prev1 = (seq_t)75;  /* a(2) */
    seq_t a_curr;

    /* Linear time recurrence; compute until n */
    for (idx_t i = 3; i <= n; ++i) {
        a_curr = (seq_t)9 * a_prev1;     /* 9*a(n-1)   */
        a_curr -= (seq_t)2 * a_prev2;    /* -2*a(n-2)  */
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }
    return a_prev1;
}
