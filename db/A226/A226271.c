/*
 * A226271  Index of 1/n in the Fibonacci (or rabbit) ordering of the positive
 *          rationals.
 * https://oeis.org/A226271
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-3) for n > 4 Initial values: a(1)=1,
 *            a(2)=4, a(3)=6, a(4)=9
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 45, u64 n <= 91, u128 n <= 184,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)6;
    if (n == 4) return (seq_t)9;

    /* Recurrence: a(n) = 2*a(n-1) - a(n-3)  (n >= 5)  */
    seq_t a_nm3 = (seq_t)4; /* a(2)   */
    seq_t a_nm2 = (seq_t)6; /* a(3)   */
    seq_t a_nm1 = (seq_t)9; /* a(4)   */
    seq_t cur;

    for (idx_t i = 5; i <= n; ++i) {
        cur = ((seq_t)2 * a_nm1) - a_nm3; /* compute a(i) */
        a_nm3 = a_nm2;                    /* shift: a(i-3) <- a(i-2) */
        a_nm2 = a_nm1;                    /* shift: a(i-2) <- a(i-1) */
        a_nm1 = cur;                      /* shift: a(i-1) <- a(i)   */
    }
    return a_nm1; /* a(n) */
}
