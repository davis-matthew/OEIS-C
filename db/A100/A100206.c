/*
 * A100206  Row sums of Clark's triangle A046902.
 * https://oeis.org/A100206
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0; a(1)=7; a(2)=20; a(n)=3*a(n-1)-2*a(n-2) for n≥3
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)7;
    if (n == 2) return (seq_t)20;

    seq_t a_nm2 = (seq_t)7;   /* a(n-2) */
    seq_t a_nm1 = (seq_t)20;  /* a(n-1) */
    for (idx_t i = 3; i <= n; ++i)
    {
        seq_t next = 3 * a_nm1 - 2 * a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = next;
    }
    return a_nm1;
}
