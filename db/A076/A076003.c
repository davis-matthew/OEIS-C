/*
 * A076003  Third column of triangle A075503.
 * https://oeis.org/A076003
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-3)*3072 - a(n-2)*704 + a(n-1)*48 with a(0)=1,
 *            a(1)=48, a(2)=1600
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is derived from the generating function 1/[(1-8x)(1-16x)(1-24x)] */

/* The function below evaluates the nth term.  It does not use any
   external helpers and works for any seq_t that supports the
   usual arithmetic operators. */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)48;
    if (n == 2) return (seq_t)1600;

    seq_t a_nm3 = (seq_t)1;     /* a(0)   */
    seq_t a_nm2 = (seq_t)48;    /* a(1)   */
    seq_t a_nm1 = (seq_t)1600;  /* a(2)   */
    seq_t a_n;                  /* current term */

    const seq_t contrib3 = (seq_t)3072;
    const seq_t contrib2 = (seq_t)704;
    const seq_t contrib1 = (seq_t)48;

    for (idx_t i = 3; i <= n; ++i)
    {
        /* a(n) = 48*a(n-1) - 704*a(n-2) + 3072*a(n-3) */
        a_n = contrib1 * a_nm1;
        a_n -= contrib2 * a_nm2;
        a_n += contrib3 * a_nm3;

        /* shift for next iteration */
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_n;
}
