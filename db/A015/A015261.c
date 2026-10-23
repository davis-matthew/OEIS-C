/*
 * A015261  Gaussian binomial coefficient [ n,2 ] for q = -10.
 * https://oeis.org/A015261
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 91*a(n-1) + 910*a(n-2) - 1000*a(n-3);
   initial values: a(2)=1, a(3)=91, a(4)=9191   (offset 2) */

seq_t a(idx_t n)
{
    if (n < 2) return 0;
    if (n == 2) return 1;
    if (n == 3) return 91;
    if (n == 4) return 9191;

    seq_t a_nm3 = 1;      /* a(n-3) */
    seq_t a_nm2 = 91;     /* a(n-2) */
    seq_t a_nm1 = 9191;   /* a(n-1) */
    seq_t val;

    for (idx_t i = 5; i <= n; ++i) {
        val = 91 * a_nm1 + 910 * a_nm2;
        val -= 1000 * a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = val;
    }
    return a_nm1;
}
