/*
 * A226857  Numbers that are both the sum of two Fibonacci numbers and the
 *          product of two Fibonacci numbers.
 * https://oeis.org/A226857
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 64, u32 n <= 133, u64 n <= 271,
 *            u128 n <= 548, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-3) + a(n-6) for n > 12
   Initial values: a(1)=0, a(2)=1, a(3)=2, a(4)=3, a(5)=4, a(6)=5,
   a(7)=6, a(8)=8, a(9)=9, a(10)=10, a(11)=13, a(12)=15. */

seq_t a(idx_t n)
{
    static const seq_t init[12] = {
        (seq_t)0, (seq_t)1, (seq_t)2, (seq_t)3, (seq_t)4, (seq_t)5,
        (seq_t)6, (seq_t)8, (seq_t)9, (seq_t)10, (seq_t)13, (seq_t)15
    };

    if (n <= 12)
        return init[n - 1];

    /* Use a sliding window over the last six terms:
       a0 = a(n-6), a1 = a(n-5), ..., a5 = a(n-1).          */
    seq_t a0 = init[6];  /* a(7)   */
    seq_t a1 = init[7];  /* a(8)   */
    seq_t a2 = init[8];  /* a(9)   */
    seq_t a3 = init[9];  /* a(10)  */
    seq_t a4 = init[10]; /* a(11)  */
    seq_t a5 = init[11]; /* a(12)  */

    for (idx_t k = 13; k <= n; ++k) {
        seq_t val = a3 + a0;   /* a(n) = a(n-3) + a(n-6) */

        /* Shift window forward by one position */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = val;
    }
    return a5; /* a(n) resides in a5 after the loop */
}
