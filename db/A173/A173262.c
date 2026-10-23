/*
 * A173262  a(n) = (2*10^n + 61)/9.
 * https://oeis.org/A173262
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2*10^n + 61)/9
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=9, a(2)=29, a(n)=11*a(n-1)-10*a(n-2) for n≥3 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* below offset – undefined behaviour */
    if (n == 1) return 9;
    if (n == 2) return 29;

    seq_t a_nm2 = 9;   /* a(n-2) */
    seq_t a_nm1 = 29;  /* a(n-1) */
    seq_t a_n;

    for (idx_t i = 3; i <= n; ++i) {
        a_n = 11 * a_nm1 - 10 * a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_nm1;
}
