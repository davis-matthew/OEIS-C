/*
 * A145304  Numbers X such that there exists Y in N : X^2=237*Y^2+79.
 * https://oeis.org/A145304
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A145304: Numbers X such that X^2 = 237*Y^2 + 79.
   Recurrence: a(1) = 3002; a(2) = 1369815602;
               a(n+2) = 456302*a(n+1) - a(n). */

seq_t a(idx_t n)
{
    /* Offset: a(1) is the first term. */
    if (n < 1) return 0;                /* undefined for n<1, return 0 */
    if (n == 1) return (seq_t)3002;
    if (n == 2) return (seq_t)1369815602;

    const seq_t C = (seq_t)456302;
    seq_t a_nm2 = (seq_t)3002;          /* a(n-2)   */
    seq_t a_nm1 = (seq_t)1369815602;    /* a(n-1)   */
    seq_t a_n     = 0;                  /* current term */

    for (idx_t i = 3; i <= n; ++i) {
        a_n = C * a_nm1 - a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
