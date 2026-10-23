/*
 * A178635  a(n) = 72*((10^n - 1)/9)^2.
 * https://oeis.org/A178635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=72, a(2)=8712, a(3)=887112,
   a(n)=111*a(n-1)-1110*a(n-2)+1000*a(n-3) for n>3 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)72;
    if (n == 2) return (seq_t)8712;
    if (n == 3) return (seq_t)887112;

    /* a(n-3), a(n-2), a(n-1) for the recurrence */
    seq_t a_nm3 = (seq_t)72;      /* a(1) */
    seq_t a_nm2 = (seq_t)8712;    /* a(2) */
    seq_t a_nm1 = (seq_t)887112;  /* a(3) */
    seq_t a_curr;

    for (idx_t i = 4; i <= n; ++i) {
        /* Compute a(n) = 111*a(n-1) - 1110*a(n-2) + 1000*a(n-3)  */
        /* Order operations to avoid underflow on unsigned types */
        a_curr = (seq_t)111 * a_nm1 + (seq_t)1000 * a_nm3;
        a_curr -= (seq_t)1110 * a_nm2;

        /* Shift for next iteration */
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_curr;
    }

    return a_nm1;
}
