/*
 * A051927  Number of independent vertex sets in the n-prism graph Y_n = K_2 X
 *          C_n (n > 2).
 * https://oeis.org/A051927
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1)+3*a(n-2)+a(n-3), a(0)=3, a(1)=1, a(2)=7
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 25, u64 n <= 50, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle initial values explicitly */
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)7;

    /* iterative recurrence for n >= 3 */
    seq_t a0 = (seq_t)3; /* a(n-3) */
    seq_t a1 = (seq_t)1; /* a(n-2) */
    seq_t a2 = (seq_t)7; /* a(n-1) */
    seq_t ai;

    for (idx_t i = 3; i <= n; ++i) {
        ai = a2 + (seq_t)3 * a1 + a0;      /* a(n) = a(n-1)+3*a(n-2)+a(n-3) */
        a0 = a1;                           /* shift window: a0 <= a1      */
        a1 = a2;                           /*       a1 <= a2              */
        a2 = ai;                           /*       a2 <= a(n)             */
    }
    return a2;
}
