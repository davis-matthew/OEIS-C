/*
 * A204102  Number of (n+1) X 5 0..2 arrays with column and row pair sums
 *          b(i,j)=a(i,j)+a(i,j-1) and c(i,j)=a(i,j)+a(i-1,j) such that
 *          b(i,j)*b(i-1,j)-c(i,j)*c(i,j-1) is nonzero.
 * https://oeis.org/A204102
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 15*a(n-1) - 270*a(n-3) + 324*a(n-4) Initial values: a(1)
 *            = 2304, a(2) = 31104, a(3) = 419904, a(4) = 5738688
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Guard against n < 1 (undefined), but specification says n ≥ 1. */
    if (n == 1) return (seq_t)2304;
    if (n == 2) return (seq_t)31104;
    if (n == 3) return (seq_t)419904;
    if (n == 4) return (seq_t)5738688;

    /* Use a sliding window of the last four terms:
          a0 = a(n-4), a1 = a(n-3), a2 = a(n-2), a3 = a(n-1)
       Initialize with the first four terms.
    */
    seq_t a0 = (seq_t)2304;   /* a(1) */
    seq_t a1 = (seq_t)31104;  /* a(2) */
    seq_t a2 = (seq_t)419904; /* a(3) */
    seq_t a3 = (seq_t)5738688;/* a(4) */

    /* Iterate from i = 5 up to n */
    for (idx_t i = 5; i <= n; ++i) {
        seq_t next = (seq_t)15 * a3 - (seq_t)270 * a1 + (seq_t)324 * a0;
        /* Shift window for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }

    return a3;
}
