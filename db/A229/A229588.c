/*
 * A229588  Number of defective 3-colorings of a 3 X n 0..2 array connected
 *          horizontally and antidiagonally with exactly one mistake, and
 *          colors introduced in row-major 0..2 order.
 * https://oeis.org/A229588
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) - 29*a(n-2) + 20*a(n-3) - 4*a(n-4) for n>6
 *            initial values: a(1)=0, a(2)=40, a(3)=264, a(4)=1620, a(5)=9156,
 *            a(6)=49848
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 1) return 0;          /* offset 1: a(1) is first term */
    if (n == 1) return 0;
    if (n == 2) return 40;
    if (n == 3) return 264;
    if (n == 4) return 1620;
    if (n == 5) return 9156;
    if (n == 6) return 49848;

    /* recurrence for n >= 7 */
    seq_t t1 = 264;   /* a(n-4) when n==7 */
    seq_t t2 = 1620;  /* a(n-3) when n==7 */
    seq_t t3 = 9156;  /* a(n-2) when n==7 */
    seq_t t4 = 49848; /* a(n-1) when n==7 */

    idx_t i;
    for (i = 7; i <= n; ++i) {
        seq_t next = 10*t4 - 29*t3 + 20*t2 - 4*t1;
        t1 = t2;
        t2 = t3;
        t3 = t4;
        t4 = next;
    }
    return t4;  /* t4 holds a(n) after the loop */
}
