/*
 * A099930  a(n) = Pell(n) * Pell(n-1) * Pell(n-2) / 10.
 * https://oeis.org/A099930
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Pell(n) * Pell(n-1) * Pell(n-2) / 10 Offset: a(3) = 1,
 *            a(4) = 12, a(5) = 174, a(6) = 2436 Recurrence for n >= 7: a(n) =
 *            12*a(n-1) + 30*a(n-2) - 12*a(n-3) - a(n-4)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3)
        return (seq_t)0;          /* undefined below the offset */

    /* Base terms for the sequence */
    const seq_t a3 = (seq_t)1;
    const seq_t a4 = (seq_t)12;
    const seq_t a5 = (seq_t)174;
    const seq_t a6 = (seq_t)2436;

    if (n == 3) return a3;
    if (n == 4) return a4;
    if (n == 5) return a5;
    if (n == 6) return a6;

    /* Iteratively apply the 4‑term recurrence until reaching n */
    seq_t a_nm4 = a3;   /* a(i-4) */
    seq_t a_nm3 = a4;   /* a(i-3) */
    seq_t a_nm2 = a5;   /* a(i-2) */
    seq_t a_nm1 = a6;   /* a(i-1) */
    seq_t a_cur;

    for (idx_t i = 7; i <= n; ++i) {
        a_cur = 12 * a_nm1 + 30 * a_nm2 - 12 * a_nm3 - a_nm4;
        a_nm4 = a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_cur;
    }

    return a_cur;
}
