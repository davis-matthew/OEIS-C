/*
 * A162484  a(1) = 2, a(2) = 8; a(n) = 2 a(n - 1) + a(n - 2) - 4*(n mod 2).
 * https://oeis.org/A162484
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1)=2; a(2)=8; a(n)=2*a(n-1)+a(n-2)-4*(n mod 2) for n>2
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 25, u64 n <= 50, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the base cases */
    if (n == 1)
        return (seq_t)2;
    if (n == 2)
        return (seq_t)8;

    /* Iteratively compute the sequence up to n. */
    seq_t a_nm2 = (seq_t)2;  /* a(n-2) */
    seq_t a_nm1 = (seq_t)8;  /* a(n-1) */
    seq_t a_cur;

    for (idx_t i = 3; i <= n; ++i)
    {
        if (i & 1)                  /* n is odd: subtract 4 */
            a_cur = (seq_t)2 * a_nm1 + a_nm2 - (seq_t)4;
        else                         /* n is even: no subtraction */
            a_cur = (seq_t)2 * a_nm1 + a_nm2;

        a_nm2 = a_nm1;
        a_nm1 = a_cur;
    }
    return a_nm1;
}
