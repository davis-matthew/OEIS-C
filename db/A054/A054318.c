/*
 * A054318  a(n)-th star number (A003154) is a square.
 * https://oeis.org/A054318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 11*(a(n-1) – a(n-2)) + a(n-3) * with a(1)=1, a(2)=5,
 *            a(3)=45. * Computes the nth term of OEIS A054318 for any n ≥ 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* offset is 1; indices <1 are undefined */

    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)45;

    /* Keep the three most recent terms for the recurrence */
    seq_t a_nm3 = (seq_t)1;  /* a(n-3) */
    seq_t a_nm2 = (seq_t)5;  /* a(n-2) */
    seq_t a_nm1 = (seq_t)45; /* a(n-1) */
    seq_t cur  = 0;

    for (idx_t i = 4; i <= n; ++i) {
        /* a(i) = 11*(a(i-1) – a(i-2)) + a(i-3) */
        cur = (seq_t)((a_nm1 - a_nm2) * (seq_t)11 + a_nm3);
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = cur;
    }

    return cur;
}
