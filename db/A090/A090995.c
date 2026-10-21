/*
 * A090995  Number of meaningful differential operations of the n-th order on
 *          the space R^10.
 * https://oeis.org/A090995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 34, u64 n <= 72, u128 n <= 147,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1) + 2*a(n-2) - a(n-3)
   Initial values: a(1)=10, a(2)=18, a(3)=32 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;               /* undefined for n<1, return 0 */
    if (n == 1) return (seq_t)10;
    if (n == 2) return (seq_t)18;
    if (n == 3) return (seq_t)32;

    seq_t a_nm3 = (seq_t)10;   /* a(n-3) */
    seq_t a_nm2 = (seq_t)18;   /* a(n-2) */
    seq_t a_nm1 = (seq_t)32;   /* a(n-1) */
    seq_t cur;

    for (idx_t i = 4; i <= n; ++i) {
        /* cur = a_nm1 + 2*a_nm2 - a_nm3 */
        cur = a_nm1 + (a_nm2 + a_nm2) - a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = cur;
    }
    return a_nm1;
}
