/*
 * A233258  Number of 3 X n 0..5 arrays with no element x(i,j) adjacent to
 *          itself or value 5-x(i,j) horizontally or antidiagonally, top left
 *          element zero, and 1 appearing before 2 3 and 4, and 2 appearing
 *          before 3 in row major order (unlabelled 6-colorings with no
 *          clashing color pairs).
 * https://oeis.org/A233258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 48*a(n-1) - 448*a(n-2) + 1024*a(n-3) for n>5 initial
 *            values: a(1)=11, a(2)=136, a(3)=4672, a(4)=166400, a(5)=6049792
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)11;
    if (n == 2) return (seq_t)136;
    if (n == 3) return (seq_t)4672;
    if (n == 4) return (seq_t)166400;
    if (n == 5) return (seq_t)6049792;

    /* Recurrence requires the last three terms:
       nm3 = a(n-3), nm2 = a(n-2), nm1 = a(n-1)   */
    seq_t nm3 = (seq_t)4672;      /* a(3) */
    seq_t nm2 = (seq_t)166400;    /* a(4) */
    seq_t nm1 = (seq_t)6049792;   /* a(5) */

    for (idx_t i = 6; i <= n; ++i) {
        seq_t next = (seq_t)(48) * nm1 + (seq_t)(1024) * nm3 - (seq_t)(448) * nm2;
        nm3 = nm2;
        nm2 = nm1;
        nm1 = next;
    }
    return nm1;
}
