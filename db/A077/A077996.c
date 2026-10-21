/*
 * A077996  Expansion of (1-x)/(1-2*x-x^2-2*x^3).
 * https://oeis.org/A077996
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 46, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A077996 – recurrence from the generating function
 *  (1-x)/(1-2x-x^2-2x^3)
 *
 *  a(0)=1, a(1)=1, a(2)=3
 *  a(n) = 2*a(n-1) + a(n-2) + 2*a(n-3)   for n ≥ 3
 *
 *  The implementation computes the recurrence iteratively, thus it
 *  works both in C11 and C++17 and does not require any dynamic
 *  allocation – seq_t is treated as an integer type supporting the
 *  usual arithmetic operators.
 */
seq_t a(idx_t n)
{
    /* Handle the first three values explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;

    /* a_nm3 = a(n-3), a_nm2 = a(n-2), a_nm1 = a(n-1)  during the loop */
    seq_t a_nm3 = (seq_t)1;   /* a(0) */
    seq_t a_nm2 = (seq_t)1;   /* a(1) */
    seq_t a_nm1 = (seq_t)3;   /* a(2) */
    seq_t cur   = (seq_t)0;   /* placeholder for a(n) */

    for (idx_t i = 3; i <= n; ++i) {
        cur = ( (seq_t)2 * a_nm1 ) + a_nm2 + ( (seq_t)2 * a_nm3 );
        /* shift the window for next iteration */
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = cur;
    }

    return cur;
}
