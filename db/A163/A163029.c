/*
 * A163029  Number of n X 3 binary arrays with all 1's connected and a path of
 *          1's from top row to bottom row.
 * https://oeis.org/A163029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A163029
   a(n) = 7*a(n-1) - 11*a(n-2) + 6*a(n-3) + a(n-4) - 7*a(n-5) + a(n-6)
   initial values: a(1)=6, a(2)=28, a(3)=144, a(4)=730, a(5)=3692, a(6)=18666 */

seq_t a(idx_t n)
{
    /* the sequence is defined for n >= 1; return 0 for n==0 */
    if (n == 0) return (seq_t)0;

    /* initial values */
    static const seq_t init[6] = { (seq_t)6, (seq_t)28, (seq_t)144,
                                   (seq_t)730, (seq_t)3692, (seq_t)18666 };

    if (n <= 6) return init[n-1];

    /* keep the last six values in the order:
       a0 = a(n-6), a1 = a(n-5), a2 = a(n-4),
       a3 = a(n-3), a4 = a(n-2), a5 = a(n-1) */
    seq_t a0 = init[0], a1 = init[1], a2 = init[2];
    seq_t a3 = init[3], a4 = init[4], a5 = init[5];

    for (idx_t i = 7; i <= n; ++i)
    {
        seq_t next = a5 * 7 - a4 * 11 + a3 * 6 + a2 - a1 * 7 + a0;
        a0 = a1; a1 = a2; a2 = a3; a3 = a4; a4 = a5; a5 = next;
    }
    return a5;
}
