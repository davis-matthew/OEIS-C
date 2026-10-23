/*
 * A191721  Permutations in S_n avoiding the patterns {4321, 34512, 45123,
 *          35412, 43512, 45132, 45213, 53412, 45312, 45231}.
 * https://oeis.org/A191721
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=1, a(2)=2, a(3)=6 Recurrence for n >= 4: a(n) =
 *            5*a(n-1) - 3*a(n-2) - 2*a(n-3) + a(n-4)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 32, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the nth term of OEIS A191721 */
seq_t a(idx_t n) {
    /* Base values for n = 0,1,2,3 */
    seq_t a0 = (seq_t)1;  /* a(n-4)  */
    seq_t a1 = (seq_t)1;  /* a(n-3)  */
    seq_t a2 = (seq_t)2;  /* a(n-2)  */
    seq_t a3 = (seq_t)6;  /* a(n-1)  */

    if (n == 0) return a0;
    if (n == 1) return a1;
    if (n == 2) return a2;
    if (n == 3) return a3;

    seq_t an;
    for (idx_t i = 4; i <= n; ++i) {
        an = (seq_t)5 * a3              /* 5*a(n-1) */
           - (seq_t)3 * a2              /* -3*a(n-2) */
           - (seq_t)2 * a1              /* -2*a(n-3) */
           + a0;                        /* + a(n-4) */
        /* shift window for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return an;
}
