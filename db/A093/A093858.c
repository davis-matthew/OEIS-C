/*
 * A093858  a(0) = 1, a(1)= 2, a(n) = (a(n+1) - a(n-1))/n, or a(n+1) = n*a(n)
 *          + a(n-1).
 * https://oeis.org/A093858
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A093858
   Recurrence: a(0)=1, a(1)=2,
   a(n+1) = n * a(n) + a(n-1)   (n ≥ 1)  */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    seq_t a_nm1 = (seq_t)1; /* a(n-1)  */
    seq_t a_n   = (seq_t)2; /* a(n)    */

    for (idx_t i = 1; i < n; ++i)    /* compute a(i+1) for i = 1 .. n-1  */
    {
        seq_t prod  = a_n * (seq_t)i; /* n * a(n)  */
        seq_t a_np1 = prod + a_nm1;   /* a(n+1)   */
        a_nm1 = a_n;
        a_n   = a_np1;
    }
    return a_n;   /* a(n) */
}
