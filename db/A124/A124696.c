/*
 * A124696  Number of base-3 circular n-digit numbers with adjacent digits
 *          differing by 1 or less.
 * https://oeis.org/A124696
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 3*a(n-1) - a(n-2) - a(n-3) for n >= 4 Initial values:
 *            a(0)=1, a(1)=3, a(2)=7, a(3)=15
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 25, u64 n <= 50, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)7;
    if (n == 3) return (seq_t)15;

    /* Sliding window: a_{i-3}, a_{i-2}, a_{i-1} */
    seq_t a_nm3 = (seq_t)3;      /* a(1) */
    seq_t a_nm2 = (seq_t)7;      /* a(2) */
    seq_t a_nm1 = (seq_t)15;     /* a(3) */
    seq_t a_n   = (seq_t)0;      /* placeholder */

    for (idx_t i = 4; i <= n; ++i) {
        /* a_n = 3*a_nm1 - a_nm2 - a_nm3 */
        a_n = a_nm1;
        a_n += a_n;          /* 2*a_nm1 */
        a_n += a_nm1;        /* 3*a_nm1 */
        a_n -= a_nm2;
        a_n -= a_nm3;

        /* Shift the window for the next step */
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_nm1;
}
