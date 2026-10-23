/*
 * A189315  Expansion of g.f. 5*(1-3*x+x^2)/(1-5*x+5*x^2).
 * https://oeis.org/A189315
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1)-5*a(n-2) for n > 2 a(0) = 5, a(1) = 10, a(2) =
 *            30
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 16, u64 n <= 33, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the initial terms */
    if (n == 0) return (seq_t)5;
    if (n == 1) return (seq_t)10;
    if (n == 2) return (seq_t)30;

    /* start recurrence with a(1) and a(2) */
    seq_t a_nm2 = (seq_t)10; /* a(n-2) == a(1)    */
    seq_t a_nm1 = (seq_t)30; /* a(n-1) == a(2)    */
    seq_t a_n   = (seq_t)0;  /* placeholder */

    for (idx_t i = 3; i <= n; ++i) {
        a_n   = (seq_t)5 * a_nm1 - (seq_t)5 * a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
