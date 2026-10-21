/*
 * A055389  a(0) = 1, then twice the Fibonacci sequence.
 * https://oeis.org/A055389
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 46, u64 n <= 92, u128 n <= 184,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=2, a(2)=2; for n≥3, a(n)=a(n−1)+a(n−2). */
seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)1;
    if (n == 1u)
        return (seq_t)2;
    if (n == 2u)
        return (seq_t)2;

    seq_t a_nm2 = (seq_t)2; /* a(1) */
    seq_t a_nm1 = (seq_t)2; /* a(2) */
    for (idx_t i = 3u; i <= n; ++i)
    {
        seq_t a_n = a_nm1 + a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_nm1;
}
