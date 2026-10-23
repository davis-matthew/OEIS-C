/*
 * A108210  Let M[n] be the 2 X 2 matrix {{0, -3}, {(n - 1), 5*(n - 1)}} and
 *          let v[1] = {0, 1}', v[n] = M[n]*v[n - 1]'. Then a[n] is the first
 *          entry of v[n].
 * https://oeis.org/A108210
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=0, a(2)=3; for n≥2,
   a(n+1) = (n-1) * (5*a(n) - 3*a(n-1)) */
seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;
    if (n == 2)
        return (seq_t)3;

    seq_t a_prev = (seq_t)0;   /* a(1) */
    seq_t a_cur  = (seq_t)3;   /* a(2) */

    for (idx_t i = 2; i < n; ++i)   /* compute a(i+1) */
    {
        seq_t temp = (seq_t)5 * a_cur - (seq_t)3 * a_prev;
        a_prev = a_cur;
        a_cur  = (seq_t)(i - 1) * temp;
    }

    return a_cur;
}
