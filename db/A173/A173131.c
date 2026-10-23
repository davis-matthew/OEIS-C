/*
 * A173131  a(n) = (Cosh[(2n-1)ArcSinh[n]])^2.
 * https://oeis.org/A173131
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (Cosh[(2n-1)ArcSinh[n]])^2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(0) = 1 directly */
    if (n == 0) {
        return (seq_t)1;
    }

    /* treat n as arbitrary‑precision integer for further calculations */
    seq_t n_se = (seq_t)n;           /* n as seq_t */
    seq_t D    = n_se * n_se + (seq_t)1;  /* D = n^2 + 1 */

    /* m = 2n - 1 (number of steps needed for B_k) */
    idx_t m = 2 * n - 1;             /* m ≥ 1 because n ≥ 1 */

    /* recurrence: B_0 = 0 , B_1 = 1 , B_{k+2} = 2*n*B_{k+1} + B_k   */
    seq_t B_prev = (seq_t)0;
    seq_t B_cur  = (seq_t)1;

    for (idx_t step = 1; step < m; ++step) {
        /* compute B_next = 2*n*B_cur + B_prev */
        seq_t twice_n = n_se * (seq_t)2;
        seq_t B_next  = twice_n * B_cur + B_prev;
        B_prev = B_cur;
        B_cur  = B_next;
    }

    /* a(n) = B_m^2 * D   (m = 2n-1) */
    seq_t sq = B_cur * B_cur;
    return sq * D;
}
