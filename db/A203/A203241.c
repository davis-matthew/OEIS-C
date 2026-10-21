/*
 * A203241  Second elementary symmetric function of the first n terms of
 *          (1,2,4,8,...).
 * https://oeis.org/A203241
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2 - 3*2^n + 4^n)/3
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(2)=2, a(3)=14, a(4)=70,
   a(n)=7*a(n-1)-14*a(n-2)+8*a(n-3) for n≥5 */

seq_t a(idx_t n)
{
    /* Offset starts at 2. For n below 2 the result is undefined
       but we simply return 0 to avoid UB.  */
    if (n < 2) return 0;

    /* Base cases */
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)14;
    if (n == 4) return (seq_t)70;

    /* Compute iteratively using the linear recurrence.  */
    seq_t a2 = (seq_t)2;   /* a(n-3)  */
    seq_t a3 = (seq_t)14;  /* a(n-2)  */
    seq_t a4 = (seq_t)70;  /* a(n-1)  */
    seq_t ai = 0;

    for (idx_t i = 5; i <= n; ++i) {
        ai = 7 * a4 - 14 * a3 + 8 * a2;
        a2 = a3;
        a3 = a4;
        a4 = ai;
    }
    return a4;  /* a4 now holds a(n) */
}
