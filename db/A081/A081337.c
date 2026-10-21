/*
 * A081337  a(n) = (8^n + 4^n)/2.
 * https://oeis.org/A081337
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8^n + 4^n)/2 Recurrence: a(0)=1, a(1)=6, a(n)=12*a(n-1)
 *            - 32*a(n-2) (n≥2)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)6;

    /* iterate using the 2‑term recurrence */
    seq_t a_nm2 = (seq_t)1;   /* a(n-2) */
    seq_t a_nm1 = (seq_t)6;   /* a(n-1) */
    seq_t a_n   = 0;          /* current value */

    for (idx_t i = 2; i <= n; ++i) {
        a_n   = a_nm1 * (seq_t)12 - a_nm2 * (seq_t)32;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_nm1;
}
