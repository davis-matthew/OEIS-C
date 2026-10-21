/*
 * A076004  Fourth column of triangle A075503.
 * https://oeis.org/A076004
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A076004
   Formula (recurrence):
   a(0)=1, a(1)=80, a(2)=4160, a(3)=179200
   a(n) = 80*a(n-1) - 2240*a(n-2) + 25600*a(n-3) - 98304*a(n-4)   (n≥4)
   The recurrence is implemented in a numerically safe way
   by factoring a common factor 8, giving:
   a(n) = 8 * (10*a(n-1) - 280*a(n-2) + 3200*a(n-3) - 12288*a(n-4)) */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)80;
    if (n == 2) return (seq_t)4160;
    if (n == 3) return (seq_t)179200;

    seq_t a0 = (seq_t)1;          /* a(n-4)   */
    seq_t a1 = (seq_t)80;         /* a(n-3)   */
    seq_t a2 = (seq_t)4160;       /* a(n-2)   */
    seq_t a3 = (seq_t)179200;     /* a(n-1)   */
    seq_t curr = 0;

    for (idx_t i = 4; i <= n; ++i) {
        curr = ((seq_t)10 * a3)  /* 10*a(n-1)  */
             - ((seq_t)280 * a2) /* 280*a(n-2) */
             + ((seq_t)3200 * a1)/* 3200*a(n-3) */
             - ((seq_t)12288 * a0);/*12288*a(n-4)*/
        curr *= (seq_t)8;        /* final factor 8 */

        /* shift window for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = curr;
    }
    return curr;
}
