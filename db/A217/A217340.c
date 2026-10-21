/*
 * A217340  Number of n X 2 arrays of occupancy after each element moves to
 *          some horizontal or vertical neighbor but without 2-loops.
 * https://oeis.org/A217340
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) - 11*a(n-2) + a(n-3) - a(n-4) Initial values:
 *            a(1)=0, a(2)=1, a(3)=7, a(4)=38
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 30, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1) {             /* a(1) = 0, if n==0 return 0 */
        return (seq_t)0;
    }
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)7;
    if (n == 4) return (seq_t)38;

    seq_t a4 = (seq_t)0;   /* a(n-4)   */
    seq_t a3 = (seq_t)1;   /* a(n-3)   */
    seq_t a2 = (seq_t)7;   /* a(n-2)   */
    seq_t a1 = (seq_t)38;  /* a(n-1)   */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t cur = (seq_t)0;
        cur = (seq_t)7 * a1;      /* 7*a(n-1) */
        cur -= (seq_t)11 * a2;    /* -11*a(n-2) */
        cur += a3;                /* +a(n-3) */
        cur -= a4;                /* -a(n-4) */
        a4 = a3;
        a3 = a2;
        a2 = a1;
        a1 = cur;
    }
    return a1;
}
