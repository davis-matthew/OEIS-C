/*
 * A101291  Sum of all numbers with n digits.
 * https://oeis.org/A101291
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1) = 45, a(2) = 4905 * Recurrence: a(n) = 110*a(n‑1) –
 *            1000*a(n‑2) for n > 2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* values below the offset are irrelevant; return 0 for safety */
    if (n == 0) return (seq_t)0;

    /* base cases */
    if (n == 1) return (seq_t)45;
    if (n == 2) return (seq_t)4905;

    /* iterate the linear recurrence */
    seq_t a_nm2 = (seq_t)45;       /* a(n‑2) */
    seq_t a_nm1 = (seq_t)4905;     /* a(n‑1) */
    seq_t a_n;                     /* current term */

    for (idx_t i = 3; i <= n; ++i) {
        a_n = (seq_t)110 * a_nm1 - (seq_t)1000 * a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
